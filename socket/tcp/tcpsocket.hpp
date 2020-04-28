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

#define MAX_LISTEN 10i

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
        
        bool Socket() {
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
    
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
        
        bool Listen(int backlog = MAX_LISTEN) {
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        
        bool Connect(const std::string &ip, const uint16_t port) {
            
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
        
        bool Accept(TcpSocket *sock, std::string *ip = NULL, 
                uint16_t *port = NULL) {
           
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
           
            int newfd = accept(_sockfd, (struct sockaddr*)&addr,
                    &len);
            if (newfd < 0) {
                perror("accept error");
                return false;
            }
            sock->_sockfd = newfd;
            if (ip != NULL) {
                
                *ip = inet_ntoa(addr.sin_addr);
            }
            if (port != NULL) {
                *port = ntohs(addr.sin_port);
            }
            return true;
        }
       
        bool Recv(std::string &buf) {
            
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
        
        bool Send(std::string &buf) {
            
            int ret = send(_sockfd, &buf[0], buf.size(), 0);
            if (ret < 0) {
                perror("send error");
                return false;
            }
            return true;
        }
        
        bool Close() {
            close(_sockfd);
            return true;
        }
};
#endif
