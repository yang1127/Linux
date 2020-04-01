#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigcb(int signum)//回调函数
{
    printf("recv signal:%d\n", signum);
}

int main()
{
    signal(SIGINT, sigcb);
    signal(40, sigcb); 
    sigset_t mask_set, old_set;//定义信号集合
    sigemptyset(&mask_set);//清空信号集合
    sigfillset(&mask_set);//添加所有信号到集合中

    sigprocmask(SIG_BLOCK, &mask_set, &old_set);//阻塞mask_set中所有信号

    printf("press enter to continue!\n");
    getchar();//不按回车，则程序的流程就卡在这里

    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);//解除阻塞
    //sigprocmask(SIG_SETBLOCK, &old_set, NULL);//解除阻塞,将原有old置回

    return 0;
}
