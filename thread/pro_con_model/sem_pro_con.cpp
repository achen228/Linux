//通过信号量实现生产者消费者模型
//通过数组实现数据缓存的环形队列

#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

#define QUEUE_MAX 5 //缓存队列的最大容量

class RingQueue
{
public:
    RingQueue(int qmax = QUEUE_MAX)
        : _queue(qmax)
        , _capacity(qmax)
        , _read(0)
        , _write(0)
    {
        sem_init(&_lock, 0, 1);
        sem_init(&_sem_data, 0, 0);
        sem_init(&_sem_idle, 0, _capacity);
    }

    ~RingQueue()
    {
        sem_destroy(&_lock);
        sem_destroy(&_sem_data);
        sem_destroy(&_sem_idle);
    }

    bool Push(const int& data)
    {
        sem_wait(&_sem_idle); //入队前判断是否有空闲空间，如果有则空闲空间计数-1，如果没有则一直阻塞等待
        sem_wait(&_lock); //加锁
        _queue[_write] = data;
        _write = (_write + 1) % _capacity;
        sem_post(&_lock); //解锁
        sem_post(&_sem_data); //入队数据之后有效空间计数+1，唤醒消费者
        return true;
    }

    bool Pop(int* data)
    {
        sem_wait(&_sem_data); //出队前判断是否有有效空间，如果没有则一直阻塞等待，如果有则有效空间计数-1
        sem_wait(&_lock); //加锁
        *data = _queue[_read];
        _read = (_read + 1) % _capacity;
        sem_post(&_lock); //解锁
        sem_post(&_sem_idle); //取出数据之后空闲空间计数+1，唤醒生产者
        return true;
    }
private:
    std::vector<int> _queue;
    int _capacity; //缓存队列的容量
    sem_t _lock; //信号量实现互斥锁
    sem_t _sem_data; //消费者取出数据之前，判断是否存在有效数据
    sem_t _sem_idle; //生产者放入数据之前，判断是否存在空闲空间
    int _read; //读指针，表示消费者从缓存队列中读取数据
    int _write; //写指针，表示生产者从缓存队列中放入数据
};

void* thr_productor(void* arg)
{
    RingQueue* queue = (RingQueue*)arg;
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
    RingQueue* queue = (RingQueue*)arg;
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
    RingQueue queue;
    pthread_t ptid[4], ctid[4];
    int ret, i;
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
