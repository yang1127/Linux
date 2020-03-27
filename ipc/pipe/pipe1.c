//通过匿名管道实现管道符
//ps -ef | grep shh
//ps与grep是兄弟进程，ps进程将结果交给grep进程使用进程间通信
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
	int pipefd[2] = {0};
	int ret = pipe(pipefd); //创建管道
	if (ret < 0)
	{
		perror("pipe error");
		return -1;
	}
	
	int pid1 = fork(); //创建子进程
	//没有判断子进程是否创建成功
	if (pid1 == 0) 
	{
		close(pipefd[0]); //那端没有使用，则关闭某端
		//进程1实现ps
		dup2(pipefd[1], 1); //重定向 将标准输出重定向管道写入端
		execlp("ps", "ps", "-ef", NULL); //execlp程序替换
		exit(0);
	}
	
	int pid2 = fork();
	if (pid2 == 0)
	{
		close(pipefd[1]);
		//进程2实现grep
		dup2(pipefd[0], 0);//将标准输入重定向管道读取端
		execlp("grep", "grep", "ssh", NULL);//从标准输入读取数据，而不是从管道
		exit(0);	
	}
	close(pipefd[0]);//管道在父进程创建，关闭父进程，在等待之前
	close(pipefd[1]);
	waitpid(pid1, NULL, 0); //管道哪一端不使用则关闭哪一端
        waitpid(pid2, NULL, 0);
	return 0;
}
