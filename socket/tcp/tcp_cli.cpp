#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "tcpsocket.hpp"

void sigcb(int signum)
{
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

    TcpSocket cli_sock;//客服端套接字
    CHECK_RET(cli_sock.Socket());//创建套接字
    CHECK_RET(cli_sock.Connect(ip, port));//连接服务器，不建议主动绑定地址信息
    while(1) {
        std::cout << "client say:";
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        CHECK_RET(cli_sock.Send(buf));//客户端发送数据

        buf.clear();
        CHECK_RET(cli_sock.Recv(buf));//客户端接收数据
        std::cout << "server say:" << buf << "\n";
    }
    //关闭套接字
    cli_sock.Close();
    return 0;
}
