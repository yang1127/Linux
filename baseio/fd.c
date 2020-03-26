#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	//int dup2(int oldfd, int newfd);
	//将newfd重定向到oldfd所指向的文件
	//若newfd本身已有打开文件，重定向时则关闭打开文件	
	int fd = open("./tmp.txt", O_RDWR);
	
	//让1也指向fd所指向的文件
	dup2(fd, 1); //将1号描述符的下标内容改成fd下标的内容
	printf("%d\n", fd);

	fflush(stdout);
	close(fd);

	return 0;	
}
