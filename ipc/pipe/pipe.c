//匿名管道的基本使用

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int pid = 0;
	int pipefd[2];//管道要创建于子进程之前 0读 1写
	int ret = pipe(pipefd);
	if (ret < 0)
	{
		perror("pipe error\n");
		return -1;
	}

	pid = fork(); //创建一个子进程
	if (pid == 0) //若为0 则为子进程
	{	
		//close(pipefd[1]);//关闭写端
		close(pipefd[0]);//关闭读端
		//子进程从管道读取数据
		sleep(5); //测试管道写特性，五秒后开始读，前五秒一直写
		char buf[1024] = {0};
		int ret = read(pipefd[0], buf, 1023); //当管道中没数据则会阻塞
		if (ret < 0)
		{
			perror("read error\n");
			return -1;
		}
		printf("child get data:[%s]\n", buf);
	}
	else //父进程
	{
		//close(pipefd[1]);//关闭写端
		sleep(3);
		close(pipefd[0]);//关闭读端
		//父进程向管道写入数据
		//sleep(10); 测试管道读特性
		while(1) //测试管道写特性
		{
			char *ptr = "hahah~";
			int ret = write(pipefd[1], ptr, strlen(ptr));
			if (ret < 0)
			{
				perror("write error\n");
				return -1;
			}	
		}
	}

	return 0;
}
