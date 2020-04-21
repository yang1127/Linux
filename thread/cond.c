#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int is_have_noodle = 0; //条件
pthread_cond_t cond_foodie; //条件变量
pthread_cond_t cond_chef; 
pthread_mutex_t mutex; //锁

void *foodie(void *arg) //入口函数
{
	while(1){
		//pthread_cond_wait(&cond_foodie, &mutex); //进行了三步操作解锁、休眠、唤醒后加锁
		//pthread_mutex_unlock(&mutex);
		//pause();
		//pthread_mutex_lock(&mutex);
		//若没有面，吃面的人应该解锁，让做面的去做面，但是解锁和休眠必须是一个原子操作
		//因为若解锁和休眠之间被打断，厨师做面唤醒
		//吃面的人的时候，吃面的人还没有休眠，就白唤醒了，但是因为没有
		//第二次唤醒，吃面的人就有可能会永久休眠
		pthread_mutex_lock(&mutex); //加锁
		while(is_have_noodle == 0) { //临界资源
			pthread_cond_wait(&cond_foodie, &mutex); //让一个线程阻塞
		}
		printf("eat noodles, delicious~~!\n");
		is_have_noodle--; //临界资源
		pthread_mutex_unlock(&mutex); //解锁
		
		//至少唤醒一个
		pthread_cond_signal(&cond_chef); //唤醒厨师 -> 各自唤醒各自等待队列上的
		//广播唤醒所有人
		//pthread_cond_brodcast(&cond_chef);

	}
	return NULL;
}

void *chef(void *arg) //入口函数
{
	while(1){
		pthread_mutex_lock(&mutex); //加锁
		while(is_have_noodle == 1) { //临界资源
			pthread_cond_wait(&cond_chef, &mutex);
		} 
		printf("cook noodles~~\n");
		is_have_noodle++; //临界资源
		pthread_mutex_unlock(&mutex); //先解锁再唤醒吃饭者
		pthread_cond_signal(&cond_foodie);
	}
	return NULL;
}

#define MAX_FOODIE 4 //吃饭者

int main (int argc, char *argv[])
{
	pthread_t foodie_tid, chef_tid; //两个线程
	int ret = 0;

	pthread_cond_init(&cond_foodie
, NULL); //初始化条件变量
	pthread_cond_init(&cond_chef, NULL);
	pthread_mutex_init(&mutex, NULL); //初始化锁
	for (int i = 0; i < MAX_FOODIE; i++) {
		ret = pthread_create(&foodie_tid, NULL, foodie, NULL); //创建线程
		if (ret != 0) {
			printf("thread create error\n");
			return -1;
		}
	}
	ret = pthread_create(&chef_tid, NULL, chef, NULL); //创建线程
	if (ret != 0) {
		printf("thread create error\n");
		return -1;
	}

	pthread_join(foodie_tid, NULL); //等待
	pthread_join(chef_tid, NULL); //等待
	pthread_cond_destroy(&cond_chef); //销毁
	pthread_cond_destroy(&cond_foodie); //销毁
	pthread_mutex_destroy(&mutex); //销毁锁
	return 0;
}
