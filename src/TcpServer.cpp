#include "TcpServer.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

TcpServer::TcpServer(int port) : port_(port), listenfd_(-1), isRunning_(false) {}

TcpServer::~TcpServer() {
    if (listenfd_ >= 0) {
        close(listenfd_);
    }
}

void TcpServer::start() {
    initServer(); 
    while (isRunning_) {
        std::cout << "Waiting for client..." << std::endl;
        int connfd = acceptClient(); 
        if (connfd < 0) {
            continue;
        }
        handleClient(connfd); 
    }
    close(listenfd_);
}

void TcpServer::initServer() {
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr.sin_port = htons(port_); 
    bind(listenfd_, (sockaddr*)&addr, sizeof(addr));
    listen(listenfd_, 5);
    isRunning_ = true;
    std::cout << "Server initialized on port " << port_ << std::endl;
}

int TcpServer::acceptClient() {
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int connfd = accept(listenfd_, (sockaddr*)&clientAddr, &clientLen);
    return connfd;
}

void TcpServer::handleClient(int connfd) {
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