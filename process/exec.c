#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//int main()
int main(int argc, char *margv[], char *env[])
{
	//execl("/bin/ls", "ls", "-l", "-a", NULL);//“ls”运行参数
	
	char *argv[32] = {NULL};
	argv[0] = "ls";
	argv[1] = "-l";
	argv[2] = NULL;
	//execv("/bin/ls", argv);
	
	/*
	if (execvp("ls", argv) < 0) { //execv需要路径、execvp不需要路径,但进限制于path环境变量，给路径也不算错,运行系统默认命令
		perror("execv error");
		return -1;
	}
	*/

	//子进程的环境变量实际上都是父进程给的
	if (execve("./env", argv, NULL) < 0) {
		perror("execv error");
		return -1;
	}
	printf("吃饭饭~~hahahah~~\n");
	return 0;
}
