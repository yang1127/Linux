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

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip, port));
    CHECK_RET(lst_sock.Listen());

    while(1) {
        TcpSocket cli_sock;
        if (lst_sock.Accept(&cli_sock) == false) {
            continue;
        }
        std::cout << "new connect: "<<cli_sock.GetFd()<<"\n";
        std::string buf;
        if (cli_sock.Recv(buf) == false) {
            cli_sock.Close();
            continue;
        }
        std::cout << "req:[" << buf << "]\n";

        std::string text;
        text="<html><body><h1>Hello World</h1></body></html>";
        std::stringstream tmp;
        tmp << "HTTP/1.1 500 Not Found\r\n";
        tmp << "Content-Length: " << text.size() << "\r\n";
        tmp << "Connection: close\r\n";
        tmp << "Content-Type: text/html\r\n";
        tmp << "Location: http://www.taobao.com/\r\n";
        tmp << "\r\n";
        tmp << text;
        cli_sock.Send(tmp.str());
        cli_sock.Close();
    }
    lst_sock.Close();
    return 0;
}
