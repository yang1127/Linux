#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class UdpSocket {
private:
    int _sockfd;
private:

    void MakeAddr(struct sockaddr_in &addr, const std::string &ip, 
                  const uint16_t port){
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
public:
    UdpSocket():_sockfd(-1) {}

    bool Socket() {
        _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (_sockfd < 0) {
            perror("socket error");
            return false;
        }
        return true;
    }

    bool Bind(const std::string &ip, const uint16_t port){
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

    bool Send(const std::string &buf, std::string &dip, uint16_t dport) {
        struct sockaddr_in addr;
        MakeAddr(addr, dip, dport);
        socklen_t len = sizeof(struct sockaddr_in);

        int ret = sendto(_sockfd, &buf[0], buf.size(), 0, 
                         (struct sockaddr*)&addr, len);
        if (ret < 0) {
            perror("sendto error");
            return false;
        }
        return true;
    }

    bool Recv(std::string &buf, 
              std::string *ip = NULL, uint16_t *port = NULL) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(struct sockaddr_in);
        char tmp[4096] = {0};

        int ret = recvfrom(_sockfd, tmp, 4095, 0, 
                           (struct sockaddr*)&addr, &len);
        if (ret < 0) {
            perror("recvfrom error");
            return false;
        }
        buf.assign(tmp, ret);
        if (ip != NULL) {

            *ip = inet_ntoa(addr.sin_addr);
        }
        if (port != NULL) {
            *port = ntohs(addr.sin_port);
        }
        return true;
    }

    bool Close() {
        close(_sockfd);
    }
};
