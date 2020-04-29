#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo) {
    while(waitpid(-1, NULL, WNOHANG) > 0); //处理僵尸进程 =0没有进程退出 <0出错 >0 处理
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "./tcp_process ip port\n";
        return -1;
    }
    signal(SIGCHLD, sigcb);//SIGCHLD非可靠信号
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    
    TcpSocket lst_sock;//服务端套接字
    CHECK_RET(lst_sock.Socket());//创建套接字
    CHECK_RET(lst_sock.Bind(ip, port));//绑定信息
    CHECK_RET(lst_sock.Listen());//开始监听
    //获取新连接
    while(1) {
        TcpSocket cli_sock;
        bool ret;
        ret = lst_sock.Accept(&cli_sock);
        if (ret == false) {
            continue;
        }
        std::cout << "new connect\n";

	//创建子进程与客户端进行通信
	//父进程永远只做一件事：获取新连接
        pid_t pid = fork();
        if (pid < 0) {
	    //关闭 继续下一个客户端
            cli_sock.Close();
            continue;
        }else if (pid == 0) {
	    //接收数据
            while(1) {
                std::string buf;
                cli_sock.Recv(buf);
                std::cout << "client say:"<<buf<<"\n";
                std::cout << "server say:";
                fflush(stdout);
		//发送数据
                buf.clear();
                std::cin>> buf;
                cli_sock.Send(buf);
            }
	    //关闭
            cli_sock.Close();
            exit(0);//不希望有下一个父进程
        }
        cli_sock.Close();//父进程关闭：父子进程数据独有
    }
    //关闭套接字
    lst_sock.Close();

    return 0;
}
