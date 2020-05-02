#include <stdlib.h>
#include <stdio.h>
#include "tcpsocket.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "./tcp_srv ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip, port));
    CHECK_RET(lst_sock.Listen());
    TcpSocket cli_sock;
    while(1) {
        std::string cli_ip;
        uint16_t cli_port;
        if (lst_sock.Accept(&cli_sock, &cli_ip, &cli_port)==false){
            continue;
        }
	int sockfd = cli_sock.GetFd();
	cal_t tmp;
	recv(sockfd, (void*)&tmp, sizeof(cal_t), 0); //0:阻塞接收
	printf("%d %c %d\n", tmp.num1, tmp.op, tmp.num2);
    }
    lst_sock.Close();
    return 0;
}
