#include<iostream>
#include<thread>
#include<shared_mutex>
#include<mutex>

static std::mutex mux;

class XMutex{
public:
    XMutex(std::mutex& mux):mux_(mux){
        mux_.lock();
        std::cout<<"Locking the mutex"<<std::endl;
    }

    ~XMutex(){
        mux_.unlock();
        std::cout<<"Unlocking the mutex"<<std::endl;
    }
private:
    std::mutex& mux_;
};

void TestMutex(int status){
    XMutex lock(mux);
    if(status == 1){
        std::cout<<"Thread 1 is locking the mutex"<<std::endl;
    }
    else{
        std::cout<<"not thread 1 is locking the mutex"<<std::endl;
    }
}

int main(){
    TestMutex(1);
    TestMutex(2);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
