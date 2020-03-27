//命名管道
//int mkfifo(const char *pathname, mode_t mode);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h> //O——RDONLY
#include <errno.h> //errno

int main()
{
	char *file = "./pipe2.fifo";
	int ret = mkfifo(file, 0644);
	if (ret < 0)
	{
		if (errno != EEXIST) //当管道文件不存在时
		{
			perror("mkfifo error");
			return -1;	
		}
	}

	//int fd = open(file, O_RDONLY);//只读方式,命名管道会阻塞，表示没人写
	int fd = open(file, O_WRONLY);//只写方式,命名管道会阻塞，表示没人读		
	//int fd = open(file, O_RDWR);//可读可写方式
	if (fd < 0)
	{
		perror("open error");
		return -1;
	}

	printf("open success\n");
	
	while(1)
	{
		char buf[1024] = {0};
		scanf("%s", &buf);
		int ret = write(fd, buf, strlen(buf));
		if (ret == 0)
		{
			printf("管道没有人读，所有读端被关闭\n");
		}
		else if (ret < 0)
		{
			perror("write error\n");
			return -1;
		}
	}

	return 0;
}

