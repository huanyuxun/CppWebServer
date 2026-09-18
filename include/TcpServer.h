#ifndef TCPSERVER_H
#define TCPSERVER_H

class TcpServer {
public:
    TcpServer(int port);
    ~TcpServer();
    void start();

private:
    void initServer();
    int acceptClient();
    void handleClient(int connfd_);

private:
    int port_;
    int listenfd_;
    bool isRunning_;
};

#endif