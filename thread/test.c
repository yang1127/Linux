//通过黄牛抢票的例子体会线程安全的重要性以及认识互斥锁
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_THR 4 //黄牛人数

int tickets = 100; //票数
pthread_mutex_t mutex; //定义mutex变量

void *cattle(void *arg) //入口函数
{
	while(1) {
		//pthread_mutex_lock    阻塞加锁
		//pthread_mutex_trylock 尝试加锁 -> 非阻塞加锁，加锁不了、报错、返回
		//pthread_mutex_timedlock 限制阻塞时长的阻塞加锁
		pthread_mutex_lock(&mutex); //加锁：在访问临界资源之前
		if (tickets > 0) { //有票才会抢票
			printf("%p i get a ticket:%d\n", pthread_self(), tickets);
			sleep(3);
			tickets--;
			pthread_mutex_unlock(&mutex);//解锁
		}else  { 
			printf("have no tickets\n");
			//加锁之后，需要在任意有可能退出线程的地方都要进行解锁
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
	return NULL;
}

int main (int argc, char *argv[])
{
	pthread_t tid[MAX_THR]; //黄牛使用线程表示

	pthread_mutex_init(&mutex, NULL); //初始化在创建线程之前
	int i, j;
	for (i = 0; i < MAX_THR; i++) {
		int ret = pthread_create(&tid[i], NULL, cattle, NULL); //创建线程
		if (ret != 0) {
			printf("thread create error\n");
			return -1;
		}
	}
	for (j = 0; j < MAX_THR; j++) {
		pthread_join(tid[j], NULL);  //线程等待
	}
	pthread_mutex_destroy(&mutex); //销毁
	return 0; //主线程退出、导致进程退出
}
