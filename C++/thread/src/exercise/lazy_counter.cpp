#include<iostream>
#include<thread>
#include<mutex>
#include<unordered_map>

class LazyCounter{
public:
    static constexpr int NUMCPUS = 20;
    LazyCounter(int threshold = 1024): threshold_(threshold){}

    void increment(int cpu_id){
        std::lock_guard<std::mutex> lck(local_mtx_[cpu_id]);
        local_count_[cpu_id]++;
        if(local_count_[cpu_id] >= threshold_){
            std::lock_guard<std::mutex> global_lck(global_mtx_);
            global_count_ += local_count_[cpu_id];
            local_count_[cpu_id] = 0;
        }
    }

    void reset(int cpu_id){
        std::lock_guard<std::mutex> lck(local_mtx_[cpu_id]);
        local_count_[cpu_id] = 0;
    }


    int get_global_count(){
        for(auto& [cpu_id, count]: local_count_){
            std::lock_guard<std::mutex> lck(local_mtx_[cpu_id]);
            global_count_ += count;
            count = 0;
        }
        return global_count_;
    }


    ~LazyCounter(){
        int global_count = get_global_count();
        std::cout << "global count: " << global_count << std::endl;
    }


private:
    std::unordered_map<int, std::mutex> local_mtx_;
    std::unordered_map<int, int> local_count_;
    std::mutex global_mtx_;
    int global_count_ = 0;
    int threshold_ ;
};

int main(){
    LazyCounter lazy_counter;
    for(int i = 0; i < LazyCounter::NUMCPUS; i++){
        std::thread t([&lazy_counter, i](){
            for(int j = 0; j < 10000000; j++){
                lazy_counter.increment(i);
            }
        });
        t.join();
    }

}