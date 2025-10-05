#include<iostream>
#include<functional>
#include<thread>
#include<mutex>
#include<atomic>
#include<condition_variable>
#include<vector>

template <typename T>
class TimeWheel;

template<typename T>
class TimeNode{
friend class TimeWheel<T>;
public:
    using CallBack = std::function<void(const T&)>;
    //链表节点的 next_ 和 pre_ 是指针成员变量，C++ 中未显式初始化的指针会指向随机内存（野指针），而非 nullptr
    //因此，在构造函数中，需要初始化 next_ 和 pre_ 为 nullptr，以避免出现野指针
    // TimeNode(size_t rotation,size_t slot,const T& data,CallBack callback):
    // rotation_{rotation},slot_{slot},data_{data},callback_{callback} {}
    TimeNode(size_t rotation,size_t slot,const T& data,CallBack callback):
    rotation_{rotation},slot_{slot},data_{data},callback_{callback},next_(nullptr),pre_(nullptr) {}

    TimeNode(const TimeNode&)=delete;
    TimeNode& operator=(const TimeNode&) = delete;

    TimeNode(TimeNode&& other) noexcept{
        rotation_ = other.rotation_;
        slot_ = other.slot_;
        data_ = other.data_;
        callback_ = other.callback_;
        next_ = other.next_;
        pre_ = other.pre_;
        other.next_ = nullptr;
        other.pre_ = nullptr;
    }
    TimeNode& operator=(TimeNode &&) noexcept{
        if(this == &other) return *this;
        rotation_ = other.rotation_;
        slot_ = other.slot_;
        data_ = other.data_;
        callback_ = other.callback_;
        next_ = other.next_;
        pre_ = other.pre_;
        other.next_ = nullptr;
        other.pre_ = nullptr;
        return *this;
    }

private:
    size_t rotation_;
    size_t slot_;
    T data_;
    CallBack callback_;
    TimeNode* next_;
    TimeNode* pre_;
};

template <typename T>
class TimeWheel{
public:
    using Duration = std::chrono::milliseconds;    //错误：注意微秒和毫秒的区别
    using CallBack = typename TimeNode<T>::CallBack;

    TimeWheel(size_t slot_count, uint64_t interval)
        : slot_count_{slot_count}, interval_{std::chrono::milliseconds(interval)}, 
          current_slot_{0}, running_{false} {
        if (slot_count == 0) {
            throw std::invalid_argument("Slot count cannot be zero");
        }
        if (interval_.count() <= 0) {
            throw std::invalid_argument("Interval must be positive");
        }
        
        nodes_.resize(slot_count_, nullptr);
    }

    ~TimeWheel() {
        stop();
        clear();
    }

    TimeWheel(const TimeWheel&) = delete;
    TimeWheel& operator=(const TimeWheel&) = delete;

    TimeWheel(TimeWheel&&) noexcept = default;
    TimeWheel& operator=(TimeWheel&&) noexcept = default;

    void start(){
        std::lock_guard<std::mutex> lock(mtx_);
        if(!running_){
            running_ = true;
            work_thread_ = std::thread(&TimeWheel::worker,this);
        }
    }

    TimeNode<T>* add_timer(uint64_t times,CallBack callback,const T& data){
        //uint64_t 是 无符号整数类型，取值范围是 0 ~ 2^64-1，不存在 “小于 0” 的情况
        // if(times < 0){
        //     throw std::invalid_argument("time must be positive");
        // }
        if(!callback){
            throw std::invalid_argument("callback cannot be null");
        }

        //错误：注意微秒和毫秒的区别
        //std::chrono::microseconds temp_times{times};
        std::chrono::milliseconds temp_times{times};
        uint64_t ticks = static_cast<uint64_t>(std::chrono::duration_cast<Duration>(temp_times) / interval_);

        // 关键：读取current_slot_前加锁，避免数据竞争
        std::lock_guard<std::mutex> lock(mtx_); //加锁（修改后添加的内容）
        size_t rotation = ticks / slot_count_;
        // 错误：current_slot_ 被 work_thread_ 动态修改（worker函数中递增），此处读取未加锁
        size_t slot = (current_slot_ + (ticks % slot_count_)) % slot_count_;
        TimeNode<T>* timeNode = new TimeNode<T>(rotation,slot,data,std::move(callback));
        // 错误：普通new失败会抛std::bad_alloc异常，不会返回nullptr
        //C++ 中 普通new（非new(nothrow)）若内存分配失败，会直接抛出 std::bad_alloc 异常
        //不会返回 nullptr。因此 if(!timeNode) 永远不会触发，这段代码冗余且误导
        //
        // if(!timeNode){
        //     throw std::runtime_error("timeNode is nullptr when !nodes_[slot]");
        // }
        // 正确：使用std::make_unique创建智能指针

        // 后续插入节点的逻辑（无需再加锁，当前锁已覆盖）
        //std::lock_guard<std::mutex> lock(mtx_);
        if(!nodes_[slot]){
            nodes_[slot] = timeNode;
        }
        else{
            TimeNode<T>* temp = nodes_[slot];
            temp->pre_ = timeNode;
            timeNode->next_ = temp;
            nodes_[slot] = timeNode;
        }
        return timeNode;
    }

    void stop(){
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if(!running_) return;
            running_ = false;
            con_var_.notify_one();
        }

        if(work_thread_.joinable()){
            work_thread_.join();
        }
    }

    void clear(){
        std::lock_guard<std::mutex> lock(mtx_);
        // 错误：auto& 无法绑定到iterator的值类型（begin()返回临时iterator，非左值）
        // 如果使用范围for循环，则可以用auto& 如：for(auto& nodes_it : nodes_)
        for(auto nodes_it = nodes_.begin();nodes_it != nodes_.end();++nodes_it){ 
            TimeNode<T>* to_remove = *nodes_it;
            TimeNode<T>* temp = nullptr;
            while(to_remove){
                temp = to_remove;
                to_remove = to_remove->next_;
                delete temp;
            }
            //在 clear() 中清空 slot 的头指针，避免野指针
            *nodes_it = nullptr;
        }
    }

    bool remove_timer(TimeNode<T>* node){
        if(!node){
            return false;
        }
        std::lock_guard<std::mutex> lock(mtx_);
        return remove_node(node->slot_,node);
    }

    bool is_running(){
        return running_;
    }

private:
    void worker(){
        if(running_){
            auto next_weak_up = std::chrono::steady_clock::now() + interval_;

            while(running_){
                std::unique_lock<std::mutex> lock(mtx_);
                if(con_var_.wait_until(lock,next_weak_up,[this]{return !running_;})){
                    break;
                }

                process_slot(current_slot_);

                current_slot_ = (current_slot_ + 1) % slot_count_;

                next_weak_up += interval_;
            }
        }
    }

    void process_slot(size_t current_slot){
        TimeNode<T>* cur_node = nodes_[current_slot];

        //cur_node == nullptr 是正常情况，不是错误
        if(!cur_node){
            //std::cerr<<"Error: cur_node is nullptr"<<std::endl;
            //std::cout<<"info: cur_node is nullptr"<<std::endl;
            return;
        }
        while(cur_node){
          if(cur_node->rotation_ > 0){
            cur_node->rotation_--;
            cur_node = cur_node->next_;
          }else{
            /* 1. 用引用绑定到节点内的data（此时节点还存在）
            const T& data = cur_node->data_;  // 注意：这里是「引用」，不是拷贝！
            CallBack callback = cur_node->callback_;
            TimeNode<T>* to_remove_node = cur_node;
            cur_node = cur_node->next_;
            
            2. 删除节点（释放节点内存，包括data的内存！）
            remove_node(current_slot, to_remove_node);  // 内部执行delete to_remove_node
            
            3. 执行回调：此时data引用的内存已被释放，访问就是“野引用”
            mtx_.unlock();
            try{
                callback(data);  // 访问已释放的内存，打印时崩溃
            }catch(...){
                std::cerr<<"Error: occur in callback(data)"<<std::endl;
            }
            try-catch无法捕获这种错误：因为 “访问已释放内存” 属于硬件级别的内存错误（如段错误），
            不是 C++ 的异常，所以catch(...)也抓不到，程序直接退出。
            */

             // 关键修正：用「值拷贝」获取data，而非引用
            T data = cur_node->data_;  // 拷贝节点的data到局部变量，生命周期独立
            CallBack callback = cur_node->callback_;
            TimeNode<T>* to_remove_node = cur_node;
            cur_node = cur_node -> next_;
            remove_node(current_slot,to_remove_node);


            //存在隐患：若回调执行时间过长（如超过 interval_），会导致时间轮的下一次 tick 延迟，影响定时精度；
            //解决方法：将回调函数的执行时间限制在一个合理的范围内，如不超过 100ms。
            mtx_.unlock();
            try{
                callback(data);
            }catch(...){
                std::cerr<<"Error: occur in callback(data)"<<std::endl;
            }
            mtx_.lock();
          }
        }
    }

    bool remove_node(size_t slot,TimeNode<T>* to_remove_node){
        // 错误：slot == slot_count_ 时会越界，但当前判断无法拦截
        //if(!to_remove_node || slot > slot_count_) return false;
        if(!to_remove_node || slot >= slot_count_) return false;

         // 错误：若头节点有next，删除后slot应指向next，而非设为nullptr
        // 若头节点无next，删除后slot应指向nullptr，而非设为nullptr
        // if(nodes_[slot] == to_remove_node){
        //     nodes_[slot] = nullptr;
        // }
        if(to_remove_node == nodes_[slot]){
            nodes_[slot] = to_remove_node->next_;
            if(nodes_[slot]){
                nodes_[slot]->pre_ = nullptr;
            }
        }
        else{
            if(to_remove_node->pre_){
                to_remove_node->pre_->next_ = to_remove_node->next_;
            }
            if(to_remove_node->next_){
                to_remove_node->next_->pre_ = to_remove_node->pre_;
            }
        }
        delete to_remove_node;
        return true;
    }
private:
    std::vector<TimeNode<T>*> nodes_;
    size_t slot_count_;
    Duration interval_;
    size_t current_slot_;

    std::thread work_thread_;
    mutable std::mutex mtx_;
    std::atomic<bool> running_;
    std::condition_variable con_var_;

};

