#include<stdio.h>
#include<stdlib.h>

//argv[] 存放程序运行参数
//argc[] 程序运行参数的个数
int main(int argc, char *argv[], char *env[])
{
	extern char **environ;
	int i;
	for (i = 0; environ[i] != NULL; i++){
		printf("environ[%d] = [%s]\n", i, environ[i]);
	}

	/*int i;
	for (i = 0; i < argc; i++){
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	for (i = 0; env[i] != NULL; i++){
		printf("env[%d] = [%s]\n", i, env[i]);
	}
	*/

	//char *getenv(const char *name)
	//通过环境变量名获取内容
	char *ptr = getenv("MYTEST");
	if (ptr == NULL){
		printf("have no MYTEST\n");
		return -1;
	}
	printf("MYTEST=%s\n", ptr);

	return 0;
}
