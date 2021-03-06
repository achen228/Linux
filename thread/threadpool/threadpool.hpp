#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>

#define THREAD_MAX 5 //线程池中线程的最大数量
typedef void(*handler_t)(int); //处理数据方法的函数指针

class ThreadTask
{
public:
    void SetTask(int data, handler_t handler)
    {
        _data = data;
        _handler = handler;
    }

    void Run()
    {
        _handler(_data);
    }
private:
    int _data; //任务中要处理的数据
    handler_t _handler; //任务中处理数据的方法
};

class ThreadPool
{
public:
    ThreadPool(int thr_max = THREAD_MAX)
        : _thr_max(thr_max) 
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond, NULL);

        for(int i = 0; i < _thr_max; i++)
        {
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
            if(ret < 0)
            {
                printf("pthread_create error\n");
                exit(1);
            }
        }
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

    bool PushTask(ThreadTask& task)
    {
        pthread_mutex_lock(&_mutex);
        _queue.push(task);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_broadcast(&_cond);
        return true;
    }

    static void* thr_start(void* arg)
    {
        ThreadPool* pthis = (ThreadPool*)arg;
        //不断的从任务中获取数据，调用Run()处理
        while(1)
        {
            pthread_mutex_lock(&pthis->_mutex);
            while(pthis->_queue.empty())
            {
                pthread_cond_wait(&pthis->_cond, &pthis->_mutex);
            }
            ThreadTask task;
            task = pthis->_queue.front();
            pthis->_queue.pop();
            pthread_mutex_unlock(&pthis->_mutex);
            //任务中处理数据的方法要放到锁外，保证多个执行流同时处理数据
            task.Run();
        }
        return NULL;
    }
private:
    int _thr_max; //线程池中线程的最大数量
    std::queue<ThreadTask> _queue;
    pthread_mutex_t _mutex; //实现线程访问任务队列的互斥锁
    pthread_cond_t _cond; //实现线程访问任务队列同步的条件变量
};
