//以黄牛抢票为例，演示多个执行流对同一临界资源进行访问，如果不保护，会造成数据的二义性

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ticket = 100;

void* thr_start(void* arg)
{
    while(1)
    {
        if(ticket > 0)
        {
            usleep(1000);
            printf("I got a ticket:%d\n", ticket);
            ticket--;
        }
        else 
        {
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main()
{
    pthread_t tid[4];
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
    return 0;
}
