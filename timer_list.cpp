#include<iostream>
#include<chrono>
#include <thread>

class TimerNode{
public:
    TimerNode(std::chrono::steady_clock::duration time, const std::string& text = "")
        : prev_{nullptr}, next_{nullptr}, expire_time_{std::chrono::steady_clock::now() + time}, text{text} {}
    TimerNode(const TimerNode&) = delete;
    TimerNode(TimerNode&& other): prev_{other.prev_},next_{other.next_},expire_time_{other.expire_time_},text{other.text}{other.next_ = nullptr;
            other.prev_ = nullptr;}
    TimerNode& operator=(const TimerNode&) = delete;
    TimerNode& operator=(TimerNode&& other){
        if(&other != this){
           text = other.text;
            next_ = other.next_;
            prev_ = other.prev_;
            expire_time_ = other.expire_time_;
           
            other.next_ = nullptr;
            other.prev_ = nullptr;
        }
        return *this;
    }

    void Todo(){
        std::cout<<"the node has worked\a she said: "<<text<<std::endl;
    }

    TimerNode* getNext(){return next_;}
    TimerNode* getPrev(){return prev_;}
    auto getExpireTime(){return expire_time_;}
    void setNext(TimerNode* next){next_ = next;}
    void setPrev(TimerNode* prev){prev_ = prev;}

   
private:
    TimerNode* prev_;
    TimerNode* next_;
    std::chrono::steady_clock::time_point expire_time_;
    std::string text;
};

class TimerHandle{
public:
    static TimerHandle& getInstance(){
        static TimerHandle instance;
        return instance;
    }

    //此处应该从虚拟头结点的下一个节点开始比较时间
    void AddNode(std::chrono::steady_clock::duration time,const std::string& text){
        TimerNode* node = new TimerNode{time,text};
        TimerNode* temp = dummyHead_->getNext();
        if(dummyHead_->getNext() == nullptr){
            Tail_ = node;
            Tail_->setPrev(dummyHead_);
            dummyHead_->setNext(Tail_);
            std::cout<<"success add tail"<<std::endl;
            return;
        }
        while(temp!=nullptr && temp->getExpireTime()<node->getExpireTime()){
            temp = temp->getNext();
        }
        if(temp == nullptr){
            node->setPrev(Tail_);
            Tail_ = node;
            std::cout<<"success add node"<<std::endl;
            return;
        }

        node->setNext(temp);
        node->setPrev(temp->getPrev());
        temp->getPrev()->setNext(node);
        temp->setPrev(node);
        std::cout<<"success add node"<<std::endl;
    }

   
    //此处一开始便携式时候有纰漏，没有考虑到删除节点是尾结点的情况
    void DelNode(TimerNode* node){
        if(!node || !dummyHead_->getNext()||node == dummyHead_){
            std::cerr<<"DelNode func error"<<std::endl;
            return;
        }
        if(node->getPrev()){
        node->getPrev()->setNext(node->getNext());
        }
        if(node->getNext()){
        node->getNext()->setPrev(node->getPrev());
        }
        if(node == Tail_){
            Tail_ = node->getPrev();
        }
        delete node;
    }

    void work(){
        TimerNode* temp = dummyHead_->getNext();
        if(temp == nullptr){
            std::cerr<<"wrok func error: no work"<<std::endl;
        }
        auto now_time = std::chrono::steady_clock::now();
        while(temp){
            if(now_time >= temp->getExpireTime()){
                temp->Todo();
                DelNode(temp);
                temp = dummyHead_->getNext();
            }
        }
    }

    ~TimerHandle(){
        TimerNode* cur = dummyHead_;
        TimerNode* temp;
        while(cur){
            temp = cur->getNext();
            delete cur;
            cur = temp;
        }
    }
private:
    TimerHandle():dummyHead_{new TimerNode{std::chrono::seconds(0)}},Tail_{nullptr}{
       dummyHead_->setNext(Tail_); 
    }

private:
    TimerNode* dummyHead_;
    TimerNode* Tail_;
};


int main(int argc,char* argv[]){
    TimerHandle& th = TimerHandle::getInstance();
    th.AddNode(std::chrono::milliseconds(100),"hello world!!!!!!!");
    th.AddNode(std::chrono::milliseconds(200),"this is 200 milliseconds");
    th.AddNode(std::chrono::milliseconds(400),"i am 400 milliseconds");
    th.AddNode(std::chrono::milliseconds(500),"this is 500");
    th.AddNode(std::chrono::milliseconds(10),"the shortest");
    th.AddNode(std::chrono::milliseconds(4550),"i am so long hsa 4550");
    
    while (1)
    {   
         th.work();
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    

    return 0;
}