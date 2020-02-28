#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = fork(); //创建子进程
	if (pid < 0) {
		perror("fork error"); //出错
		return -1;
	}else if (pid == 0) { //==0为子进程
		sleep(5);
		exit(99);
	}
	//wait(NULL);
	int status; //1位
	while (waitpid(-1, &status, WNOHANG) == 0) {
		printf("吃饭饭~~~hahahah\n");
		sleep(1);
	}
	if (status & 0x7f) {
		printf("program error\n");
		return -1;
	}
	if (WIFEXITED(status)) {
		printf("exit code:%d\n", WEXITSTATUS(status));
	}
	printf("exit child code:%d\n", (status >> 8) & 0xff);

	while(1) {
		printf("-------\n");
		sleep(1);
	}
	return 0;
}

