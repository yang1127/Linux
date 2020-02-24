//僵尸进程
//
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int gval = 100;
int main()
{
	int ppid = getpid();
	//exit(0); //退出、则不会进行下一步操作
	int pid = fork();
	//fork 创建子进程之后，父子进程代码共享，数据独有
	if (pid > 0){
		//parent
		gval = 10;
	}else if (pid == 0){
		//child
		sleep(3);
		exit(0); //子进程退出则为僵尸进程
	}
	
	printf("--- 你好漂亮！---%d, pid=%d-ppid:%d\n", gval, getpid(), ppid);//getpid():当前调用进程id、ppid:父进程，打印出来相同为父进程，不同为子进程
	while(1){
		sleep(1);
	}

	return 0;
}
