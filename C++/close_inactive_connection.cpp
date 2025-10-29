//使用map，将<sock_fd,timeout>作为键值存储起来
//在后台遍历map,寻找超时链接,如果超时,则断开
//创建好线程后使用detach将其分离,注意使用锁,避免资源竞争

//对于所有的连接都有一个最大过期时间（即MAX_DURATION）
//在后台创建一个线程，循环扫描已建立的链接对象
//根据对象里记录的连接建立时间，与当前时间做差值
//如果差值大于MAX_DURARION则断开连接，否则继续遍历

#include<iostream>
#include<mutex>
#include<chrono>
#include<thread>
#ifdef _WIN32
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#endif
#include<map>
#include<atomic>
class ConnectManager{
public:
    ConnectManager(const ConnectManager&) = delete;
    ConnectManager(ConnectManager&&) = delete;
    ConnectManager& operator=(const ConnectManager&) = delete;
    ConnectManager& operator=(ConnectManager&&) = delete;

    static ConnectManager& getInstance(){
        static ConnectManager cn;
        return cn;
    }

    void addConnection(int fd){
        std::lock_guard<std::mutex> lock(mtx_);
        auto now = std::chrono::steady_clock::now();
        map_[fd] = now;
    }

    void modifyConnection(int fd){
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = map_.find(fd);
        if(it != map_.end()){
            auto now = std::chrono::steady_clock::now();
            it->second = now;
        }
    }

    void clearConnection(){
        std::lock_guard<std::mutex> lock(mtx_);
        if(!cleaner_started_){
            cleaner_started_ = true;
            std::thread([this]{
                while(running_){
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                    for(auto it = map_.begin(); it != map_.end(); ) {
                        auto now = std::chrono::steady_clock::now();
                        auto duration = now - it->second;
                        if(duration > MAX_DURATION){
                    #ifdef _WIN32
                            closesocket(it->first);
                    #else
                            close(it->first);
                    #endif

                            it = map_.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
            }).detach();
        }
    }

    ~ConnectManager(){
        running_ = false;
    }
private:
    std::chrono::steady_clock::duration MAX_DURATION = std::chrono::seconds(50);
    std::mutex mtx_;
    std::atomic<bool> running_{true};
    std::map<int,std::chrono::steady_clock::time_point> map_;
    bool cleaner_started_{false};
    ConnectManager() = default;
};