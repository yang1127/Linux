#include <stdlib.h>
#include <stdio.h>
#include "tcpsocket.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "./tcp_cli ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());
    CHECK_RET(cli_sock.Connect(ip, port));
    int sockfd = cli_sock.GetFd();
    while(1) {
	cal_t tmp; //定义结构体
	tmp.num1 = 11;
	tmp.num2 = 22;
	tmp.op = '+';
	send(sockfd, (void*)&tmp,sizeof(cal_t), 0);	
    }
    cli_sock.Close();
    return 0;
}
