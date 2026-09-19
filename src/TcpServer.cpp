#include "TcpServer.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

TcpServer::TcpServer(int port) : port_(port), socketfd_(-1) {}

TcpServer::~TcpServer() {
    if (socketfd_ >= 0) close(socketfd_);
}

void TcpServer::start() {
    socket_();
    bind_();
    listen_();
    while (true) {
        int connfd = accept_();
        handle_(connfd);
    }
}

//创建一个标准的IPv4 Tcp socket
void TcpServer::socket_() {
    socketfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd_ == -1) {
        //logger:socket err
        std::cout << "socket创建失败：" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    //logger:socket suc
}

//绑定
void TcpServer::bind_() {
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr)); 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port_);
    my_addr.sin_addr.s_addr = htons(INADDR_ANY);
    int ret = bind(socketfd_, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if (ret == -1) {
        //logger:bind err
        std::cout << "bind绑定失败：" << strerror(errno) << std::endl;// 如果出错，这里就会捕获到 EACCES 或 EADDRINUSE
        exit(EXIT_FAILURE);
    }
    //logger:bind suc
}

//监听
void TcpServer::listen_() {
    int res = listen(socketfd_, 5);
    if (res == -1) {
        //logger:listen err
        std::cout << "listen失败：" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    //logger:listen suc
}

//连接
int TcpServer::accept_() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connfd = accept(socketfd_, (sockaddr*)&client_addr, &client_len);
    if (connfd == -1) {
        //logger:accept err
        std::cout << "accept失败：" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    //logger:accept suc
    return connfd;
}

void TcpServer::handle_(int connfd) {
    char buf[1024]; 
    while (true) {
        memset(buf, 0, sizeof(buf)); 
        int len = read(connfd, buf, sizeof(buf));
        if (len > 0) {
            std::cout << "Recv from client: " << buf << std::endl;
            write(connfd, buf, len);
        }
        else if (len == 0) {
            std::cout << "Client closed the connection." << std::endl;
            close(connfd);
            break; 
        }
        else {
            std::cerr << "read error: " << strerror(errno) << std::endl;
            close(connfd);
            break;
        }
    }
}