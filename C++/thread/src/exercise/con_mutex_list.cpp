#include<condition_variable>
#include<thread>
#include<mutex>
#include<atomic>
#include<list>
#include<iostream>
#include<sstream>
#include<string>

template <typename T>
class XThread{
public:
    virtual void Main() = 0;
    virtual void Start(){
        if(!is_run_){
            is_run_ = true;
            th_ = std::thread(&XThread<T>::Main, this);
        }
    }

    virtual void Wait(){
        if(th_.joinable()){
            th_.join();
        }
        
    }

    virtual void Stop(){
        if(is_run_){
            is_run_ = false;
            Wait();
        }
    }

    bool isRun(){return is_run_;}

private:
    std::thread th_;
 
    std::atomic<bool> is_run_ = false;
};

template <typename T>
class TestThread: public XThread<T>{
public:
    void Main() override {
        while (isRun()) {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this](){ return !mess_.empty(); });
            T msg = mess_.front();
            mess_.pop_front();
            lock.unlock();
            // if constexpr (std::is_same<T, std::string>::value) {
            //     if (msg == "exit") break;
            // }
            std::cout << std::this_thread::get_id() << " message is: " << msg << std::endl;
        }
    }

    void sendMessage(const T& m){
        std::unique_lock<std::mutex> lock(mtx_);
        mess_.emplace_back(m);
        cv_.notify_one();
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::list<T> mess_;
};

int main(){
    TestThread<std::string> th;
    th.Start();
    for(int i = 0;i< 10;i++){
        std::stringstream ss;
        ss<<"hello "<<std::this_thread::get_id()<<" "<<i*i;
        th.sendMessage(ss.str());
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    th.Stop();
}

