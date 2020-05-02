//实现网络版计算器
#ifndef __M_TCP_H__
#define __M_TCP_H__
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q)==false){return -1;}

#define MAX_LISTEN 10

typedef struct {
    int num1;
    int num2;
    char op;
}cal_t;

class TcpSocket
{
    private:
        int _sockfd;
    private:
        void MakeAddr(struct sockaddr_in &addr, 
                const std::string &ip, const uint16_t port){
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
        }
    public:
        TcpSocket():_sockfd(-1) {}
        int GetFd() {
            return _sockfd; //通过描述符操作
        }
        //1. 创建套接字
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        //2. 绑定地址信息
        bool Bind(const std::string &ip, const uint16_t port) {
            struct sockaddr_in addr;
            MakeAddr(addr, ip, port);
            socklen_t len = sizeof(struct sockaddr_in);

            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        //3. 开始监听（服务端）
        bool Listen(int backlog = MAX_LISTEN) {
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        //3. 发起连接请求（客户端）
        bool Connect(const std::string &ip, const uint16_t port) {
            // int connect(int sockfd, struct sockaddr*, socklen_t)
            struct sockaddr_in addr;
            MakeAddr(addr, ip, port);
            socklen_t len = sizeof(struct sockaddr_in);

            int ret;
            ret = connect(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("connect error");
                return false;
            }
            return true;
        }
        //4. 获取新连接（服务端）
        bool Accept(TcpSocket *sock, std::string *ip = NULL, 
                uint16_t *port = NULL) {
            //int accept(int sockfd, struct sockaddr* cliaddr, 
            //socklen_t len);
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            //获取新连接描述符，并且获取这个新连接对应的客户端地址
            int newfd = accept(_sockfd, 
                    (struct sockaddr*)&addr, &len);
            if (newfd < 0) {
                perror("accept error");
                return false;
            }
            sock->_sockfd = newfd;
            std::cout << "accept fd: " << newfd << "\n";
            if (ip != NULL) {
                //inet_ntoa 将网络字节序的整数IP转换为字符串
                *ip = inet_ntoa(addr.sin_addr);
            }
            if (port != NULL) {
                *port = ntohs(addr.sin_port);
            }
            return true;
        }
        //5. 接收数据
        bool Recv(std::string &buf) {
            //ssize_t recv(int sockfd, char*buf, int len, int flag)
            char tmp[4096] = {0};
            int rlen = recv(_sockfd, tmp, 4096, 0);
            if (rlen < 0) {
                perror("recv error");
                return false;
            }else if (rlen == 0) {
                std::cerr << "peer shutdown\n";
                return false;
            }
            buf.assign(tmp, rlen);
            return true;
        }
        //6. 发送数据
        bool Send(const std::string &buf) {
            //ssize_t send(int fd, char *buf, int len, int flag)
            int ret = send(_sockfd, &buf[0], buf.size(), 0);
            if (ret < 0) {
                perror("send error");
                return false;
            }
            return true;
        }
        //7. 关闭套接字
        bool Close() {
            close(_sockfd);
            return true;
        }
};
#endif
