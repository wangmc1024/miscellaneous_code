#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<list>
#include<chrono>
#include<sstream>

class XThread{

public:
    virtual void Start(){
        std::cout<<"start thread"<<std::endl;
        is_exit_ = false;
        thread_ = std::thread(&XThread::Main,this);
    }

    virtual void Stop(){
        if(!is_exit_){
            std::cout<<"stop thread"<<std::endl;
            //需要先设置is_exit_为true,否则会死锁
            is_exit_ = true;
            Wait();
        }
    }

    virtual void Wait(){
        std::cout<<"wait thread"<<std::endl;
        if(thread_.joinable()){
            thread_.join();
        }
    }

    bool is_exit(){
        return is_exit_;
    }

private:
    virtual void Main() = 0;
    std::atomic<bool> is_exit_;
    std::thread thread_;
    std::mutex mtx_;
};



class XMsgServer: public  XThread{
public:
    void Main() override{
         while(!is_exit()){
             std::unique_lock<std::mutex> lock(mtx_);
             
             if(msgs_.empty()) {
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }

             while(!msgs_.empty()){
                std::string msg = msgs_.front();
                msgs_.pop_front();
                std::cout<<std::this_thread::get_id()<<" message is: "<<msg<<std::endl;
             }
        }
    }

    void sendMesgs(const std::string& msg){
       std::unique_lock<std::mutex> lock(mtx_);
       msgs_.emplace_back(msg);
    }

private:
    std::list<std::string> msgs_;
    std::mutex mtx_;
};

int main(int argc, char* argv[]){
    XMsgServer server;
    server.Start();
    for(int i = 0; i < 10; i++){
        std::ostringstream oss;
        oss<<"message "<<i*i;
        server.sendMesgs(oss.str());
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    server.Stop();

}