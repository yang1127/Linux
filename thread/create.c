
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg) //线程入口函数
{
	sleep(5);
	pthread_exit(NULL); 
	//pthread_detach(pthread_self());
	sleep(5);
	//char ptr[] = "mingtianxiujia~~\n";
	char *ptr = "mingtianxiujia~~\n";
	pthread_exit(ptr);
	while(1) {
		printf("child thread----%s\n", (char*)arg);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid; //线程id
	char *ptr = "this is thread\n"; 
	int ret = pthread_create(&tid, NULL, thr_start, (void*)ptr); //char强转为void*
	if (ret != 0)
	{
		perror("thread create error");
		return -1;
	}
        //pthread_detach(tid);
        char *retval;
        pthread_join(tid, (void**)&retval);
        printf("retval:%s\n", retval); 
        //sleep(5);
        //pthread_cancel(tid);

	while(1)
	{
		printf("main thread----%d\n", tid);
		sleep(1);
	}

	return 0; 
}
