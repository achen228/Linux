//通过黄牛抢票的例子来体会信号量的使用

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int ticket = 100;
sem_t sem;

void* thr_start(void* arg)
{
    (void)arg;
    while(1)
    {
        sem_wait(&sem);
        if(ticket > 0)
        {
            usleep(1000);
            printf("I got a ticket:%d\n", ticket);
            ticket--;
            sem_post(&sem);
        }
        else 
        {
            sem_post(&sem);
            pthread_exit(NULL);
        }
    }
    return NULL;
}
int main()
{
    pthread_t tid[4];
    int ret, i;

    sem_init(&sem, 0, 1);

    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&tid[i], NULL, thr_start, NULL);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return 0;
        }
    }

    for(i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem);
    return 0;
}
