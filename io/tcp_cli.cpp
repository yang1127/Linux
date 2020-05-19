#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "tcpsocket.hpp"

void sigcb(int signum) {
    printf("recv a signum:%d\n", signum);
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "./tcp_cli ip port\n";
        return -1;
    }
    signal(SIGPIPE, sigcb);
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());
    CHECK_RET(cli_sock.Connect(ip, port));
    while(1) {
        std::cout << "client say:";
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        cli_sock.Send(buf);

        buf.clear();
        cli_sock.Recv(buf);
        std::cout << "server say:" << buf << "\n";
    }
    cli_sock.Close();
    return 0;
}
