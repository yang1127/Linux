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

    TcpSocket lst_sock;//监听套接字
    CHECK_RET(lst_sock.Socket());//创建套接字
    CHECK_RET(lst_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//开始监听
    //获取新连接
    TcpSocket cli_sock;//不知道哪个客户端到来 -> 造成阻塞
    while(1) {//不能可只获取一次 使用while
        std::string cli_ip;
        uint16_t cli_port;
        if (lst_sock.Accept(&cli_sock, &cli_ip, &cli_port)==false){
            continue;
        }
	//cli_sock用于与指定的客户端进行通信
	//lst_sock只用于获取新连接	
        std::cout <<"new connect: "<< cli_ip<<":"<<cli_port<<"\n";
        //接收客户端数据
	std::string buf;
        bool ret = cli_sock.Recv(buf);
        if (ret == false) {
	    //客服端关闭，继续找下一个客户端
            cli_sock.Close();
            continue;
        } 
        std::cout << "client say: " << buf << "\n";
        std::cout << "server say:";
        fflush(stdout);
        buf.clear();//清空buf
        std::cin >> buf;
	//给客户端发送数据
        ret = cli_sock.Send(buf);
        if (ret == false) {
            cli_sock.Close();
            continue;
        }
    }
    //关闭套接字
    lst_sock.Close();
    return 0;
}
