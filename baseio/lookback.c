#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int ret;
	FILE *fp = NULL; //句柄
	/*
	   fp = fopen("./tmp.txt", "a+"); //a:追加只写，不能读数据 //a+:可读可写可追加
	   if (fp == NULL)
	   {
	   perror("fopen error"); //perror:系统调用接口，系统错误问题
	   return -1;
	   }
	   */

	fp = fopen("./tmp.txt", "r+"); //r+:可读可写
	if (fp == NULL)
	{
		perror("fopen error"); 
		return -1;
	}

	char *ptr = "hahah~~~!\n";
	//char *ptr = "hello xiaoyang!\n";
	//size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);//size_t size：一块数据多大,size_t nmemb写多少块
	ret = fwrite(ptr, strlen(ptr), 1, fp );//ptr:写入的数据
	printf("write item:%d\n", ret);

	//int fseek(FILE *stream, long offset, int whence); //偏移值、冲哪里偏移SEEK_SET开始位置、SEEK_CUR当前位置、SEEK_END结束位置
	//fseek(fp, 0, SEEK_SET);
	fseek(fp, -10, SEEK_SET); //-10:向前偏移十个位置

	char buf[1024] = {0};
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);//size_t size：一块数据多大,size_t nmemb写多少块
	ret = fread(buf, 1023, 1, fp);
	printf("read buf:[%s]-ret:[%d]\n", buf, ret);

	fclose(ptr);

	return 0;
}
