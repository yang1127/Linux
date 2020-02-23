//简单的进度条，仅每次换行
#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[11] = {0};
	int i = 0;
	for (; i < 10; i++){
		buf[i] = '-';
		printf("%s\r", buf);
		 //\n换行符:数据换行，针对标准输出的时候，还有刷新缓冲区的作用
		 //\r:回车符:让光标移动到起始位置
		fflush(stdout);//刷新标准输出的缓冲区
		sleep(1);
	}
	return 0; 
}
