#include "TcpServer.h"

int main() {
    TcpServer server(8888);
    server.start();
    return 0;
}
