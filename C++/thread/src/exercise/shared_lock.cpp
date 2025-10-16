//shared_lock needs C++14 or later
//shared_lock needs to work with unique_lock or lock_guard

#include <iostream>
#include <mutex>
#include <shared_mutex>

static std::shared_timed_mutex st_mtx;

int main(){
    //read lock shared_lock
    {
        //call lock_shared() to get shared_lock
        std::shared_lock<std::shared_timed_mutex> lock(st_mtx);
        std::cout << "Thread 1: Locked shared_timed_mutex" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //exit stack, unlock automatically
    }

    //write lock exclusive_lock 
    {
        //call lock() to get exclusive_lock
        std::unique_lock<std::shared_timed_mutex> lock(st_mtx);
        std::cout << "Thread 2: Locked shared_timed_mutex" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //exit stack, unlock automatically
    }
}