#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
int main()
{
	//kill(getpid(), SIGQUIT);//向任意进程发送任意信号
	//raise(SIGQUIT);//给进程自身发送信号
	// abort();//给自身发送SIGABRT信号，引发进程非正常终止
	//alarm(3);

	while(1) {
		printf("--------------------\n");
		sleep(10);
	}
	return 0;
}
