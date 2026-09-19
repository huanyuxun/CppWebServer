#pragma once

class TcpServer {
public:
    TcpServer(int port);
    ~TcpServer();
    void start();

private:
    void socket_();//创建socket
    void bind_();//命名socket，绑定地址
    void listen_();//监听socket
    int accept_();//接受连接
    void handle_(int connfd);

private:
    int port_;
    int socketfd_;
};
