// 通过信号量实现生产者与消费者模型

#include <iostream>
#include <vector>
#include <semaphore.h>
#include <pthread.h>
using std::cout;
using std::endl;
#define MAX_QUEUE 10

class RingQueue
{
public:
    RingQueue(int capacity = MAX_QUEUE)
        : _capacity(capacity)
        , _queue(capacity)
    {
        sem_init(&_lock, 0, 1);
        sem_init(&_idle_space, 0, _capacity);
        sem_init(&_data_space, 0, 0);
    }

    ~RingQueue()
    {
        sem_destroy(&_lock);
        sem_destroy(&_idle_space);
        sem_destroy(&_data_space);
    }

    // 生产者-->插入
    bool QueuePush(int data)
    {
        sem_wait(&_idle_space); // 同步-->判断是否有资源可以操作

        sem_wait(&_lock); // 互斥-->保护临界资源的操作
        _queue[_write] = data;
        _write = (_write + 1) % _capacity;
        sem_post(&_lock);

        sem_post(&_data_space); // 数据空间+1;唤醒消费者取数据 
        return true;
    }

    // 消费者-->取出
    bool QueuePop(int &data)
    {
        sem_wait(&_data_space); // 同步-->判断是否有资源可以操作

        sem_wait(&_lock); // 互斥-->保护临界资源的操作
        data = _queue[_read];
        _read = (_read + 1) % _capacity;
        sem_post(&_lock);

        sem_post(&_idle_space); // 空闲空间计数+1;唤醒生产者
        return true;
    }

private:
    std::vector<int> _queue;
    int _capacity;
    int _read;
    int _write;
    sem_t _lock;
    sem_t _idle_space; // 生产者入队数据之前判断队列中是否有空闲空间-->判断能否入队数据
    sem_t _data_space; // 消费者获取数据之前判断有数据的空间有多少-->判断能否获取数据
};

void *productor(void *arg)
{
    RingQueue *q = (RingQueue*)arg;
    int i = 0;
    while(1)
    {
        cout << "productor put a data:" << i << endl;
        q->QueuePush(i++);
    }
    return NULL;
}

void *consumer(void *arg)
{
    RingQueue *q = (RingQueue*)arg;
    while(1)
    {
        int data;
        q->QueuePop(data);
        cout << "consumer get a data:" << data << endl;
    }

    return NULL;
}

int main()
{
    pthread_t p_tid[4], c_tid[4];
    RingQueue q;
    int i;
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&p_tid[i], NULL, productor, (void*)&q);
        if(ret != 0)
        {
            cout << "pthread create error" << endl;
            return -1;
        }
    }
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&c_tid[i], NULL, consumer, (void*)&q);
        if(ret != 0)
        {
            cout << "pthread create error" << endl;
            return -1;
        }
    }
    
    for(i = 0; i < 4; i++)
    {
        pthread_join(p_tid[i], NULL);
    }
    for(i = 0; i < 4; i++)
    {
        pthread_join(c_tid[i], NULL);
    }

    return 0;
}
