// 这个demo体会posix标准信号量的基本操作

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

int ticket = 100;
sem_t sem;

void *thr_strat(void *arg)
{
    while(1)
    {
        sem_wait(&sem);
        if(ticket > 0)
        {
            printf("get a ticket:%d\n", ticket);
            ticket--;
        }
        else 
        {
            sem_post(&sem);
            pthread_exit(NULL);
        }
        sem_post(&sem);
    }
    return NULL;
}

int main()
{
    sem_init(&sem, 0, 1);

    pthread_t tid[4];
    int i;
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&tid[i], NULL, thr_strat, NULL);
        if(ret != 0)
        {
            printf("pthread creat error\n");
            return -1;
        }
    }

    for(i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem);
    return 0;
}
