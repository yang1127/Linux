#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{   
    //通过参数获取ip地址和端口号
    if (argc != 3) {
        printf("./udp_srv ip port\n");
        return -1;
    }
    uint16_t port = atoi(argv[2]);
    char *ip = argv[1];

    //创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //ipv4、udp
    if (sockfd < 0) {
        perror("socket error");
        return -1;
    }

    //为套接字绑定地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); //htons：将uint_16类型的数据转换为网络字节序
    addr.sin_addr.s_addr = inet_addr(ip); //inet_addr：将点分十进制ip字符串地址转换为网络字节序的整数ip地址
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if (ret < 0) {
        perror("bind error\n");
        return -1;
    }

    //接收数据
    while(1) {
        char buf[1024] = {0};
        struct sockaddr_in cliaddr;
        ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len); //0：默认阻塞接收
        if (ret < 0) {
            perror("recvfrom error");
            return -1;
        }
        printf("client say: %s\n", buf);
        printf("server say:");
        fflush(stdout);
        memset(buf, 0x00, 1024);
        scanf("%s", buf);

        //发送数据
        ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
        if (ret < 0) {
            perror("sendto error");
            return -1;
        }
    }

    //关闭套接字
    close(sockfd);
    return 0;
}
