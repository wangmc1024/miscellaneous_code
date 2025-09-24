#include<iostream>
#include<mutex>
#include<chrono>
#include<thread>

std::timed_mutex t_mux;

void ThreadTime(int i){
    while(1){
        if(!t_mux.try_lock_for(std::chrono::milliseconds(300))){
            std::cout<<"Thread "<<i<<"try lock for timeout"<<std::endl;
            continue;
        }
        std::cout<<i<<" get the lock"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        t_mux.unlock();
        std::cout<<i<<" release the lock"<<std::endl;
    }

}

int main(int argc,char* argv[]){
    for(int i = 0; i< 3; ++i){
        std::thread t(ThreadTime,i);
        t.detach();
    }
    getchar();
    return 0;
}