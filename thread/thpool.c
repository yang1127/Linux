#include <iostream>
#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef void (*TaskHandler)(int data);
class ThreadTask { //任务类
    private:
        int _data;
        TaskHandler _handler;
    public:
	ThreadTask(){}
        //构造函数
        ThreadTask(int data, TaskHandler handler):_data(data),
            _handler(handler) {}
	//设置任务接口
        void SetTask(int data, TaskHandler handler) {
            _data = data;
            _handler = handler;
        }
	//向外提供Run接口
        void Run() {
            _handler(_data);
        }
};

#define MAX_THR 5
class ThreadPool {
    private:
        int thr_num;//线程池中线程的最大数量
        std::queue<ThreadTask> _task_queue;//队列
        pthread_mutex_t _mutex;//保护队列安全操作
        pthread_cond_t _cond_con;//实现队列的同步操作--消费者
        pthread_cond_t _cond_pro;//实现队列的同步操作--生产者
    private:
        static void *thr_start(void *arg)//入口函数 为什么加static 类成员函数第一个参数为隐藏的this指针，而入口函数要求参数为void *arg
        {
	    //设置成static无法访问类成员函数，故传this指针
            ThreadPool *pool = (ThreadPool*)arg;
            while(1) {
		//pthread_mutex_lock(&_mutex); //类的私有成员无法访问，在public中提供公有接口访问
                pool->ThreadLock();//加锁
                while(pool->TaskIsEmpty()) {
                    pool->ThreadWait();//阻塞等待
                }
                ThreadTask tt;
                pool->QueuePop(tt); //出队
                pool->ThreadUnLock();//解锁
                tt.Run();//执行任务
            }
            return NULL;
        }
    public:
	//将接口均封装
	//出队
        bool QueuePop(ThreadTask &tt) {
            std::cout << _task_queue.size() << "\n";
            tt = _task_queue.front();
            _task_queue.pop();
            return true;
        }
	//阻塞等待接口
        void ThreadWait() {
            pthread_cond_wait(&_cond_con, &_mutex);
        }
	//唤醒接口
        void ThreadWakeUp() {
            pthread_cond_signal(&_cond_con);
        }
	//任务为空接口
        bool TaskIsEmpty() {
            return _task_queue.empty();
        }
	//加锁接口
        void ThreadLock() {
            pthread_mutex_lock(&_mutex);
        }

        void ThreadUnLock() {
            pthread_mutex_unlock(&_mutex);
        }
    public:
	//构造函数
        ThreadPool(int max_thr = MAX_THR): thr_num(max_thr){}
	//初始化->放于初始化之前，谁先运行不一定
        bool PoolInit() {
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_cond_con, NULL);
            pthread_cond_init(&_cond_pro, NULL);
            for (int i = 0; i < thr_num; i++) {
                pthread_t tid;
                int ret = pthread_create(&tid,NULL,thr_start,(void*)this); //创建线程
                if (ret != 0) {
                    std::cerr << "create thread error\n";
                    return false;
                }
                pthread_detach(tid);//线程分离：退出自动释放资源
            }
            return true;
        }
	//向外提供一个入队的接口函数
        bool TaskPush(ThreadTask &tt) {
            pthread_mutex_lock(&_mutex);//加锁
            _task_queue.push(tt);//任务入队 -> 缺点：没有上限、有风险
            pthread_mutex_unlock(&_mutex);//解锁
            pthread_cond_signal(&_cond_con);//唤醒消费者
            return true;
        }
};

void test(int data) 
{
    srand(data);//产生随机数
    int sec = rand() % 5; //取5以内的数
    printf("thread:%p--get data:%d , sleep %d sec\n", pthread_self(), data, sec);
    sleep(sec);
}

int main()
{
    ThreadPool pool;
    pool.PoolInit();//初始化
    for (int i = 0; i < 10; i++) {
        ThreadTask task(i, test);
        pool.TaskPush(task);//入队
    }
    while(1) {
        sleep(1);
    }
    return 0;
}
