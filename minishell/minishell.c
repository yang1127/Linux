//1. 获取标准输入
//2. 对数据进行解析-得到要运行的所有命令参数信息
//[   ls   -l  ]   ->  [ls] [-l]
//3. 创建子进程，进行程序替换，让子进程运行ls
//4. 父进程进行进程等待

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>

int main()
{
	while(1) {
		printf("[yzq@localhost]# "); //模拟
		fflush(stdout);  //刷新缓冲区
		char buf[1024] = {0};  //初始化
		fgets(buf, 1023, stdin); //从标准输入获取用户敲击的命令
		buf[strlen(buf) - 1] = '\0'; //字符串结尾标志
		printf("cmd:[%s]\n", buf); //打印

		//解析
		int argc = 0;
		char *argv[32] = {NULL};
		char *ptr = buf;
		while(*ptr != '\0') {
			if (!isspace(*ptr)) { //isspace：检测空白字符
				argv[argc] = ptr; //把输入的buf给与
				argc++;
				while(!isspace(*ptr) && *ptr != '\0') { //buf不为空
					ptr++;
				}
				*ptr = '\0'; //置空
			} 
			ptr++; //为空继续往下走
		}
		argv[argc] = NULL; //结尾
		int i;
		for (i = 0; i < argc; i++) {
			printf("[%s]\n", argv[i]); //输出信息
		}

		//创建子进程
		int pid = fork();
		if (pid == 0) { //子进程
			execvp(argv[0], argv); //指定目录下的程序
			exit(0); //失败则退出
		}
		waitpid(-1, NULL, 0); //父进程等待任意进程退出
	}
	return 0;
}

