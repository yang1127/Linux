#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int tickets = 100;
sem_t sem; //信号量变量

void *thr_eat (void *arg) //吃的入口函数
{
    while(1) {
        sem_wait(&sem); //判断当前可不可以吃 -1加锁
        if (tickets > 0) {
            usleep(1000);
            printf(" i get tickets:%d\n", tickets--);
            sem_post(&sem); //解锁 +1
        }else {
            sem_post(&sem); //解锁 在任何线程退出的位置都需要解锁
            pthread_exit(NULL); //线程退出
        }
    }
    return NULL;
}

void *thr_cook(void *arg) //做的入口函数
{
    while(1) {
        printf("cook noodle~~\n");
        sleep(1);
        sem_post(&sem); //解锁
    }
    return NULL;
}

int main()
{
    int ret;
    pthread_t tid1, tid2; 

    sem_init(&sem, 0, 1); //初始化信号量 0：线程 1：设置的初值
    ret = pthread_create(&tid1, NULL, thr_eat, NULL);
    if (ret != 0) {
        return -1;
    }
    ret = pthread_create(&tid2, NULL, thr_eat, NULL);
    if (ret != 0) {
        return -1;
    }

    pthread_join(tid1, NULL); //线程等待
    pthread_join(tid2, NULL);
    sem_destroy(&sem); //信号量销毁
    return 0;
}
