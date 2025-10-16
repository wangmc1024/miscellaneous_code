#include<thread>
#include<mutex>


static std::mutex mtx;
int main(){
    {
        std::unique_lock<std::mutex> lock(mtx);
        lock.unlock();
        //temporary unlock
        lock.lock();
    }

    {
        //adopt lock
        //current thread already holds the lock, but we want to adopt it
        //so we can unlock it later
        //the parameter "_Owns" is set to true to indicate that we own the lock
        mtx.lock();
        std::unique_lock<std::mutex> lock(mtx,std::adopt_lock);
    }

    {
        //the parameter "_Owns" is set to false to indicate that we don't own the lock
        std::unique_lock<std::mutex> lock(mtx,std::defer_lock);
        //lock is not acquired yet
        //do something else
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //lock is acquired here
        lock.lock();
        //do something with the lock
    }

    {
        std::unique_lock<std::mutex> lock(mtx,std::try_to_lock);
        
        if(lock.owns_lock()){
            //lock is acquired
            //do something with the lock
        }else{
            //lock is not acquired
            //do something else
        }
    }
}