#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	printf("今天有些小糟糕！");
	sleep(3);
	_exit(0);//_exit:直接退出，释放所有资源
	exit(0); //exit:退出刷新缓冲区
	return 0;// return:退出进程时，刷新缓冲区，把缓冲区文件写到文件中
}
