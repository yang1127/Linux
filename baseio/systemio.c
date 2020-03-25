//系统调用接口

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	//mode_t umask(mode_t mask);
	umask(0); //修改调用进程的文件创建权限掩码
	//int open(const char *pathname, int flags, mode_t mode)
	//mode & (~umask)
	//int fd = open("./sys.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
	int fd = open("./sys.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	if (fd  < 0)
	{
		perror("open error");
		return -1;
	}

	//ssize_t write(int fd, const void *buf, size_t count);
	//fd:打开文件所返回的文件描述符
	//buf:要向文件写入的数据
	//count:要写入的数据长度
	//返回值:实际写入的字节数，错误返回-1
	char buf[1024] = "yanghahah~\n";
	int ret = write(fd, buf, strlen(buf));
	if (ret < 0)
	{
		perror("write error");
		return -1;
	}
	
	//off_t lseek(int fd, off_t offset, int whence);
	//fd:打开文件所返回的文件描述符
	//offset:偏移量
	//whence:偏移量位置 SEEK_SET、SEEK_CUR、SEEK_END
	//返回值:返回当前位置到文件起始位置的偏移量 
	lseek(fd, 0, SEEK_SET);	

	//ssize_t read(int fd, void *buf, size_t count);
	//fd:打开文件所返回的文件描述符
	//buf:对读取到的数据进行存储的位置
	//count:要写入的数据长度
	//返回值:实际的写入字节数， 错误返回-1
	memset(buf, 0x00, 1024);
	ret = read(fd, buf, 1023);
	if (ret < 0)
	{
		perror("read error");
		return -1;
	}
	printf("read buf:[%s]\n", buf);
	
	//int close(int fd);
	close(fd);

	return 0;
}
