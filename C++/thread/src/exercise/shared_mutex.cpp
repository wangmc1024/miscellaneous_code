#include <iostream>
#include <shared_mutex>
#include <thread>

std::shared_mutex st_mux;
void ThreadWrite(int i){
    while(1){
        //模拟读操作
        st_mux.lock_shared();
        std::cout << "Thread " << i << " reads: " << i << std::endl;
        st_mux.unlock_shared();
        
        //模拟写操作
        st_mux.lock();
        std::cout << "Thread " << i << " writes: " << i*i << std::endl;
        st_mux.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ThreadRead(int i){
    while(1){
        //模拟读操作    
        st_mux.lock_shared();
        std::cout << "Thread " << i << " reads: " << i << std::endl;    
        st_mux.unlock_shared(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc,char* argv[]){
    for(int i = 0; i < 3; ++i){
        std::thread t1(ThreadRead,i);
        t1.detach();
    }
    for(int i = 0; i < 3; ++i){
        std::thread t1(ThreadWrite,i);
        t1.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
