// 这个demo实现生产者与消费者模型
// 模型的实现主要在于线程安全的队列操作

#include <iostream>
#include <queue>
#include <pthread.h>
#define MAX_QUEUE 10

using std::cout;
using std::endl;

class BlockQueue
{
public:
    // 构函数-->互斥锁和条件变量的初始化
    BlockQueue(int capacity = MAX_QUEUE)
        : _capacity(capacity)
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond_product, NULL);
        pthread_cond_init(&_cond_consumer, NULL);
    }

    // 析构函数-->线程销毁
    ~BlockQueue() 
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond_product);
        pthread_cond_destroy(&_cond_consumer);
    }

    // 生产者-->插入
    bool QueuePush(int data)
    {
        pthread_mutex_lock(&_mutex);
        while(((int)_queue.size()) == _capacity)
        {
            pthread_cond_wait(&_cond_product, &_mutex);
        }
        _queue.push(data);
        pthread_cond_signal(&_cond_consumer);
        pthread_mutex_unlock(&_mutex);

        return true;
    }

    // 消费者-->取出
    bool QueuePop(int &data)
    {
        pthread_mutex_lock(&_mutex);
        while(_queue.empty())
        {
            pthread_cond_wait(&_cond_consumer, &_mutex);
        }
        data = _queue.front();
        _queue.pop();
        pthread_cond_signal(&_cond_product);
        pthread_mutex_unlock(&_mutex);

        return true;
    }

private:
    std::queue<int> _queue;
    int _capacity; // 用于限制队列中最大结点数量
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_product;
    pthread_cond_t _cond_consumer;
};

void *thr_consumer(void *arg)
{
    BlockQueue *q = (BlockQueue*)arg;
    while(1)
    {
        int data;
        q->QueuePop(data);
        cout << "consumer:" << pthread_self() << " get data:" << data << endl;
    }

    return NULL;
}

void *thr_product(void *arg)
{
    BlockQueue *q = (BlockQueue*)arg;
    int i = 0;
    while(1)
    {
        q->QueuePush(i++);
        cout << "product:" << pthread_self() << " put data:" << i << endl;
    }

    return NULL;
}

int main()
{
    pthread_t con_tid[4], pro_tid[4];
    BlockQueue q;

    // 线程创建
    int i;
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&con_tid[i], NULL, thr_consumer, (void*)&q);
        if(ret != 0)
        {
            cout << "thread create error" << endl;
            return -1;
        }
    }
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&pro_tid[i], NULL, thr_product, (void*)&q);
        if(ret != 0)
        {
            cout << "thread create error" << endl;
            return -1;
        }
    }

    // 线程等待
    for(i = 0; i < 4; i++)
    {
        pthread_join(con_tid[i], NULL);
    }
    for(i = 0; i < 4; i++)
    {
        pthread_join(pro_tid[i], NULL);
    }
    
    return 0;
}
