#include <chrono>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef __linux__
#include <sys/epoll.h>
#endif
#include <iostream>

const std::chrono::steady_clock::duration TIMEOUT = std::chrono::milliseconds(5000);
// 预计算超时毫秒数，避免重复转换
const int TIMEOUT_MS = std::chrono::duration_cast<std::chrono::milliseconds>(TIMEOUT).count();

int main() {
    // 假设已初始化epfd、events和MAX_EVENTS_NUM
    int epfd = -1;
    struct epoll_event events[1024];  // 示例大小
    const int MAX_EVENTS_NUM = 1024;

    int timeout = TIMEOUT_MS;  // 初始超时时间
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

    while (true) {
        start = std::chrono::steady_clock::now();
        int event_count = epoll_wait(epfd, events, MAX_EVENTS_NUM, timeout);

        // 处理epoll_wait错误
        if (event_count < 0) {
            if (errno != EINTR) {  // 非信号中断的错误才退出
                std::perror("epoll_wait failed");
                break;
            }
            // 信号中断则继续等待，不修改超时时间
            continue;
        }
        // 处理超时情况
        else if (event_count == 0) {
            // 超时后重置超时时间
            timeout = TIMEOUT_MS;
            // 可以添加超时处理逻辑
            continue;
        }

        // 计算实际等待时间（毫秒）
        end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // 更新剩余超时时间
        timeout -= static_cast<int>(elapsed);
        // 确保超时时间不会为负
        if (timeout < 0) {
            timeout = 0;
        }

        // 处理事件（示例）
        // handle_events(events, event_count);
    }

    return 0;
}