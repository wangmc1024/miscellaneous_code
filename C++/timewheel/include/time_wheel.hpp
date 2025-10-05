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
    TimeNode(size_t rotation,size_t slot,const T& data,CallBack callback):
    rotation_{rotation},slot_{slot},data_{data},callback_{callback} {}

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
    using Duration = std::chrono::microseconds;
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
        if(times < 0){
            throw std::invalid_argument("time must be positive");
        }
        if(!callback){
            throw std::invalid_argument("callback cannot ba null");
        }

        std::chrono::microseconds temp_times{times};
        uint64_t ticks = static_cast<uint64_t>(std::chrono::duration_cast<Duration>(temp_times) / interval_);

        size_t rotation = ticks / slot_count_;
        size_t slot = (current_slot_ + (ticks % slot_count_)) % slot_count_;
        TimeNode<T>* timeNode = new TimeNode<T>(rotation,slot,data,std::move(callback));
        if(!timeNode){
            throw std::runtime_error("timeNode is nullptr when !nodes_[slot]");
        }

        std::lock_guard<std::mutex> lock(mtx_);
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
        for(auto& nodes_it = nodes_.begin();nodes_it != nodes_.end();++nodes_it){ 
            TimeNode<T>* to_remove = *nodes_it;
            TimeNode<T>* temp = nullptr;
            while(to_remove){
                temp = to_remove;
                to_remove = to_remove->next_;
                delete temp;
            }

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
        if(!cur_node){
            return;
        }
        while(cur_node){
          if(cur_node->rotation_ > 0){
            cur_node->rotation_--;
            cur_node = cur_node->next_;
          }else{
            const T& data = cur_node->data_;
            CallBack callback = cur_node->callback_;
            TimeNode<T>* to_remove_node = cur_node;
            cur_node = cur_node -> next_;
            remove_node(current_slot,to_remove_node);

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
        if(!to_remove_node || slot > slot_count_) return false;

        if(nodes_[slot] == to_remove_node){
            nodes_[slot] = nullptr;
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

