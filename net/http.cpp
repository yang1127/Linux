#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include "tcpsocket.hpp"

int main(int argc ,char *argv[])
{
    if (argc != 3) {
        std::cerr << "./http ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock; //实例化对象
    CHECK_RET(lst_sock.Socket());//创建套接字
    CHECK_RET(lst_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//开始监听

    while(1) {
        TcpSocket cli_sock;
        if (lst_sock.Accept(&cli_sock) == false) {//客户端获取新连接
            continue;
        }
        std::cout << "new connect: "<<cli_sock.GetFd()<<"\n";
        std::string buf;
        if (cli_sock.Recv(buf) == false) {//客户端接收数据
            cli_sock.Close();
            continue;
        }
        std::cout << "req:[" << buf << "]\n";

        std::string text;
        text="<html><body><h1>Hello World</h1></body></html>";
        std::stringstream tmp;
        tmp << "HTTP/1.1 500 Not Found\r\n"; //首行
        tmp << "Content-Length: " << text.size() << "\r\n";//头部，要什么写什么
        tmp << "Connection: close\r\n";//短连接
        tmp << "Content-Type: text/html\r\n";
        tmp << "Location: http://www.taobao.com/\r\n";
        tmp << "\r\n";//空行
        tmp << text;//正文
        cli_sock.Send(tmp.str());//发送数据
        cli_sock.Close();//关闭短连接
    }
    lst_sock.Close();//关闭
    return 0;
}
