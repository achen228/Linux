//以黄牛抢票为例，演示多个执行流对同一临界资源进行访问，如果不保护，会造成数据的二义性
//加锁保护后

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 100;
pthread_mutex_t mutex;

void* thr_start(void* arg)
{
    while(1)
    {
        //只对临界资源加锁，否则会影响程序的效率
        pthread_mutex_lock(&mutex);
        if(ticket > 0)
        {
            usleep(1000);
            printf("I got a ticket:%d\n", ticket);
            ticket--;
            pthread_mutex_unlock(&mutex);
        }
        else 
        {
            //在所有可能退出的地方都必须解锁，否则会造成死锁
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main()
{
    pthread_t tid[4];

    //互斥锁的初始化一定要放在线程创建之前
    pthread_mutex_init(&mutex, NULL);

    int i, ret;
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&tid[i], NULL, thr_start, NULL);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }
    }

    for(i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }

    //互斥锁的销毁必须是在所有流程都不再使用锁的情况下
    pthread_mutex_destroy(&mutex);
    return 0;
}
