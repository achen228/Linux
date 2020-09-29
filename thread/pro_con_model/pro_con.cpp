//生产者消费者模型

#include <iostream>
#include <cstdio>
#include <queue>
#include <pthread.h>

#define QUEUE_MAX 5 //缓存队列的最大容量

class BlockQueue
{
public:
    BlockQueue(int queue_max = QUEUE_MAX)
        : _capacity(queue_max)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&_pro_cond, NULL);
        pthread_cond_init(&_con_cond, NULL);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&_pro_cond);
        pthread_cond_destroy(&_con_cond);
    }

    bool Push(const int& data)
    {
        //生产者向缓存队列中放入数据
        pthread_mutex_lock(&mutex);
        while(_queue.size() == _capacity)
        {
            pthread_cond_wait(&_pro_cond, &mutex);
        }
        _queue.push(data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&_con_cond);
        return true;
    }

    bool Pop(int* data)
    {
        //消费者从缓存队列中获取数据
        pthread_mutex_lock(&mutex);
        while(_queue.empty())
        {
            pthread_cond_wait(&_con_cond, &mutex);
        }
        *data = _queue.front();
        _queue.pop();
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&_pro_cond);
        return true;
    }
private:
    std::queue<int> _queue; //线程安全的缓存队列
    size_t _capacity; //队列的容量
    pthread_mutex_t mutex;
    pthread_cond_t _pro_cond; //生产者等待队列
    pthread_cond_t _con_cond; //消费者等待队列
};

void* thr_productor(void* arg)
{
    BlockQueue* queue = (BlockQueue*)arg;
    int i = 0;
    while(1)
    {
        queue->Push(i);
        printf("I produced a data:%d\n", i++);
    }
    return NULL;
}

void* thr_consumer(void* arg)
{
    BlockQueue* queue = (BlockQueue*)arg;
    while(1)
    {
        int data;
        queue->Pop(&data);
        printf("I consumed a piece of data:%d\n", data);
    }
    return NULL;
}

int main()
{
    pthread_t ptid[4], ctid[4];
    int ret, i;
    BlockQueue queue;
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&queue);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }

        ret = pthread_create(&ctid[i], NULL, thr_consumer, (void*)&queue);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }
    }

    for(i = 0; i < 4; i++)
    {
        pthread_join(ptid[i], NULL);
        pthread_join(ctid[i], NULL);
    }
    return 0;
}
