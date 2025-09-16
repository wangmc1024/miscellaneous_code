#include<iostream>
#if defined(_WIN32) || defined(_WIN64)
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#else if defined(__linux__) || defined(__unix__)
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#endif


int main(int argc,char* argv[]){

    #if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);  // 初始化 2.2 版本
    if (wsaErr != 0) {
        std::cerr << "WSAStartup 失败: " << wsaErr << std::endl;
        return EXIT_FAILURE;
    }
    #endif


    const char* host = "www.bilibili.com";
    const char* service = "https";

    struct addrinfo hints, *res;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host,service,&hints,&res);
    if(err != 0){
        std::cerr<<"getaddrinfo error: "<<gai_strerror(err)<<std::endl;
        return EXIT_FAILURE;
    }

    struct addrinfo* temp = res;
    while(temp!= nullptr){
        void* addr;
        char ip[INET6_ADDRSTRLEN];
        int port;

        if(temp->ai_family == AF_INET){
            struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(temp->ai_addr);
            addr = &ipv4->sin_addr;
            port = ntohs(ipv4->sin_port);
            inet_ntop(AF_INET,addr,ip,INET_ADDRSTRLEN);
            std::cout<<"IPv4: "<<ip<<":"<<port<<std::endl;
        }else if(temp->ai_family == AF_INET6){
            struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(temp->ai_addr);
            addr  = &ipv6->sin6_addr;
            port = ntohs(ipv6->sin6_port);
            inet_ntop(AF_INET6,addr,ip,INET6_ADDRSTRLEN);
            std::cout<<"IPv6: "<<ip<<":"<<port<<std::endl;
        }
        temp = temp->ai_next;
    
        }

        freeaddrinfo(res);
    #if defined(_WIN32) || defined(_WIN64)
    WSACleanup();  // 释放资源
    #endif

    return 0;
}

