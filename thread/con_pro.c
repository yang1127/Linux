#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_QUE 10

class BlockQueue {
    private:
        std::queue<int> _queue;//动态增长的队列
        int _capacity;//确定队列的最大容量
        pthread_cond_t _cond_productor;//生产者等待队列
        pthread_cond_t _cond_consumer;//消费者等待队列
        pthread_mutex_t _mutex;//用于实现互斥
    public:
	//构造函数
        BlockQueue(int que_num = MAX_QUE):_capacity(que_num) {
            pthread_mutex_init(&_mutex, NULL); //初始化
            pthread_cond_init(&_cond_consumer, NULL);
            pthread_cond_init(&_cond_productor, NULL);
        }
	
	//析构函数
        ~BlockQueue() {
            pthread_mutex_destroy(&_mutex); //销毁
            pthread_cond_destroy(&_cond_consumer);
            pthread_cond_destroy(&_cond_productor);
        }
        
	//入队 -> 提供给生产者的接口
        bool QueuePush(int &data) {
            pthread_mutex_lock(&_mutex);//_queue是临界资源需要加锁保护
            while (_queue.size() == _capacity) {//判断队列节点是否添加满了 -> while支持并发
                pthread_cond_wait(&_cond_productor, &_mutex); //条件变量等待：生产者阻塞，让一个线程阻塞,将PCB加入条件变量的阻塞队列
            }
            _queue.push(data);
            pthread_mutex_unlock(&_mutex); //解锁
            pthread_cond_signal(&_cond_consumer); //唤醒消费者
            return true;
        }
        
	//出队 -> 提供给消费者的接口
        bool QueuePop(int &data) {
            pthread_mutex_lock(&_mutex); //加锁
            while (_queue.empty()) {
                pthread_cond_wait(&_cond_consumer, &_mutex); //消费者等待
            }
            data = _queue.front(); //队首数据
            _queue.pop(); //出队
            pthread_mutex_unlock(&_mutex); // 解锁
            pthread_cond_signal(&_cond_productor); //唤醒生产者
            return true;
        }
};

void *thr_consumer(void *arg) //消费者入口函数
{
    BlockQueue *queue = (BlockQueue*)arg; //强转为(BlockQueue*)arg
    while(1){
        int data; //从队列里面取数据，而队列作为一个参数((void*)&queue)传入
        queue->QueuePop(data); //出队
        printf("**get a data:%d\n", data);
    }
    return NULL;
}

void *thr_productor(void *arg) //生产者入口函数
{
    int i = 0;
    BlockQueue *queue = (BlockQueue*)arg;
    while(1){
        queue->QueuePush(i); //入队
        printf("--put a data:%d\n", i++);
    }
    return NULL;
}

#define MAX_THREAD 4 //线程数
int main()
{
    int i, ret;
    pthread_t ctid[MAX_THREAD], ptid[MAX_THREAD]; //条件变量
    BlockQueue queue; //传入的数据均在队列

    for(i = 0; i < MAX_THREAD; i++) {
        ret = pthread_create(&ctid[i], NULL, thr_consumer, (void*)&queue); //创建消费者线程 (void*)&queue传入参数 -> 临界资源
        if (ret != 0) {
            std::cerr << "thread create error\n";
            return -1;
        }
    }
    for(i = 0; i < MAX_THREAD; i++) {
        ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&queue); //创建生产者队列
        if (ret != 0) {
            std::cerr << "thread create error\n";
            return -1;
        }
    }
    for(i = 0; i < MAX_THREAD; i++) { 
        pthread_join(ctid[i], NULL); //线程等待
        pthread_join(ptid[i], NULL); //线程等待
    }
    return 0;
}
