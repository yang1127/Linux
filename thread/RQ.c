#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_QUE 10
class RingQueue{
    public:
        std::vector<int> _queue;//使用数组实现环形队列
        int _capacity;//初始化环形队列的节点数量
        int _read_step;//当前读位置的数组下标
        int _write_step;//当前写位置的数组下标
        sem_t _sem_lock;//用户实现互斥的锁
        sem_t _sem_con;//消费者等待队列，计数器对数据空间进行数据计数
        sem_t _sem_pro;//生产者等待队列，计数器对空闲空间进行数据计数
    public:
	//构造
        RingQueue(int max_que = MAX_QUE):_capacity(max_que),
            _queue(max_que), _read_step(0), _write_step(0) {
	    //初始化信号量
            sem_init(&_sem_lock, 0, 1); //初始化
            sem_init(&_sem_con, 0, 0); //数据空间计数开始为0
            sem_init(&_sem_pro, 0, max_que); //空闲空间计数开始为当前数组大小 -> 开始均为空
        }

	//析构
        ~RingQueue() {
	    //销毁信号量
            sem_destroy(&_sem_lock); //销毁
            sem_destroy(&_sem_con);
            sem_destroy(&_sem_pro);
        }

	//两个接口 -> 入队
        bool QueuePush(int &data)
	{
		//数据入队
		//1.判断是否能够访问临界资源
		sem_wait(&_sem_pro); //对空闲数据空间计数-1，判断生产者是否还能生产数据，若<0,表示没有，生产者等待在该队列上
		//2.加锁 ->只保护临界资源
		sem_wait(&_sem_lock);
		//3.数据的入队操作
		_queue[_write_step] = data;
		_write_step = (_write_step + 1) % _capacity;
		//4.数据资源的空间计数+1，环形消费者
		sem_post(&_sem_con); //唤醒消费者，有数据空间计数+1
		//5.解锁
		sem_post(&_sem_lock);
		return true;
	}

	//出队
        bool QueuePop(int &data)
	{
		sem_wait(&_sem_con);
		sem_wait(&_sem_lock);
		data = _queue[_read_step];
		_read_step = (_read_step + 1) % _capacity;
		sem_post(&_sem_pro); //唤醒生产者，空闲空间计数+1
		sem_post(&_sem_lock);
		return true;
 
	}
};

void *consumer(void *arg)
{
	RingQueue *queue = (RingQueue*)arg;
	while(1){
		int data;
		queue->QueuePop(data);
		printf("consumer get data:%d\n", data);
	}
	return NULL;
}

void *productor(void *arg)
{
	RingQueue *queue = (RingQueue*)arg;
	int i = 0;
	while(1){
		queue->QueuePush(i);
		printf("productor get data:%d\n", i++);
	}
	return 0;
}

#define MAX_THR 4
int main()
{
	pthread_t ctid[MAX_THR], ptid[MAX_THR];
	RingQueue queue;

	for (int i = 0; i < MAX_THR; i++){
		int ret = pthread_create(&ctid[i], NULL, consumer, (void*)&queue);
		if (ret != 0){
			printf("thread create error\n");
			return -1;
		}
	}

	for (int i = 0; i < MAX_THR; i++){
		int ret = pthread_create(&ptid[i], NULL, productor, (void*)&queue);
		if (ret != 0){
			printf("thread create error\n");
			return -1;
		}
	}
	
	for (int i = 0; i < MAX_THR; i++)
	{
		pthread_join(ctid[i], NULL);
		pthread_join(ptid[i], NULL);
		return 0;
	}
	
	return 0;
}
