#ifndef __M_TCP_H__//防止头文件重复定义 -> 定义时与系统的区别
#define __M_TCP_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q)==false){return -1;}//错误检测

#define MAX_LISTEN 10

class TcpSocket
{
    private:
        int _sockfd;//封装、操作句柄

    private:
        void MakeAddr(struct sockaddr_in &addr, const std::string &ip, const uint16_t port){
            addr.sin_family = AF_INET; //ipv4地址域
            addr.sin_port = htons(port);//转换为网络字节序
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
        }

    public:
        TcpSocket():_sockfd(-1) {} //构造函数
        
	//创建套接字
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//ipv4 tcp tcp协议
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
    
	//绑定地址信息
        bool Bind(const std::string &ip, const uint16_t port) {//ip地址 端口号
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
        
	//开始监听（服务端）
        bool Listen(int backlog = MAX_LISTEN) {
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        
	//发起连接请求（客户端）
        bool Connect(const std::string &ip, const uint16_t port) {//服务端地址
            //int Connect(int sockfd, struct sockaddr*, socklen_t len);
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
        
	//获取新连接（服务端）
        bool Accept(TcpSocket *sock, std::string *ip = NULL, uint16_t *port = NULL) {
            //int Accept(int sockfd, struct sockaddr* cliaddr, socklen_t len);
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            //获取新连接描述符，并且获取这个新连接对应的客户端地址
            int newfd = accept(_sockfd, (struct sockaddr*)&addr, &len);
            if (newfd < 0) {
                perror("accept error");
                return false;
            }
	    //获取成功
            sock->_sockfd = newfd;
            if (ip != NULL) {
                //ntoa将网络字节序的整数IP转为字符串 -> 针对ipv4  ipv6 -> ntop
                *ip = inet_ntoa(addr.sin_addr);
            }
            if (port != NULL) {
                *port = ntohs(addr.sin_port);
            }
            return true;
        }
       
	//接收数据
        bool Recv(std::string &buf) {
            //ssize_t revc(int sockfd, char*buf, int len, int flag) 
            char tmp[4096] = {0};
            int rlen = recv(_sockfd, tmp, 4096, 0);
            if (rlen < 0) {
                perror("recv error");
                return false;
            }else if (rlen == 0) {//连接断开
                std::cerr << "peer shutdown\n";
                return false;
            }
            buf.assign(tmp, rlen);//成功
            return true;
        }
        
	//发送数据
        bool Send(std::string &buf) {
            //ssize_t send(int sockfd, char *buf, int len, int flag)
            int ret = send(_sockfd, &buf[0], buf.size(), 0);//0:默认阻塞
            if (ret < 0) {
                perror("send error");
                return false;
            }
            return true;
        }
        
	//关闭套接字
        bool Close() {
            close(_sockfd);
            return true;
        }
};
#endif
