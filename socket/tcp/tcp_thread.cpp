#include <stdlib.h>
#include <stdio.h>
#include "tcpsocket.hpp"

void *thr_start(void *arg)//入口函数
{
    TcpSocket *cli_sock = (TcpSocket*)arg;
    while(1) {
        std::string buf;
	//接收数据
        bool ret = cli_sock->Recv(buf);
        if (ret == false) {
            cli_sock->Close();
            break;
        } 
        std::cout << "client say: " << buf << "\n";
        std::cout << "server say:";
        fflush(stdout);
        buf.clear();
        std::cin >> buf;
	//发送数据
        ret = cli_sock->Send(buf);
        if (ret == false) {
            cli_sock->Close();
            break;
        }
    }
    cli_sock->Close();
    delete cli_sock;//释放堆上的空间，手动释放  栈->出了作用域就销毁
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "./tcp_srv ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock;//服务端套接字
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip, port));
    CHECK_RET(lst_sock.Listen());
    while(1) {
        TcpSocket *cli_sock = new TcpSocket();//相当于在堆上申请一块空间，并不会释放

	//cli_sock：用于与指定的客户端进行通信
	//lst_sock：只用于获取新连接
        std::string cli_ip;
        uint16_t cli_port;
        if (lst_sock.Accept(cli_sock, &cli_ip, &cli_port)==false){
            continue;
        }
        std::cout <<"new connect: "<< cli_ip<<":"<<cli_port<<"\n";
        pthread_t tid;
	//创建线程
        pthread_create(&tid, NULL, thr_start, (void*)cli_sock);
	//分离线程
        pthread_detach(tid);
	//线程之间不能随意释放资源 -> 资源共享

    }
    lst_sock.Close();
    return 0;
}
