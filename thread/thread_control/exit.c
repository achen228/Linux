#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thr_start(void* arg)
{
    while(1)
    {
        printf("I am a normal thread:%d\n", (*(int*)arg));
        sleep(1);
        //线程退出
        //pthread_exit(NULL);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    int num = 9527;
    int ret = pthread_create(&tid, NULL, thr_start, (void*)&num);
    if(ret != 0)
    {
        printf("pthread_create failed\n");
        return -1;
    }

    //终止指定线程，让指定线程退出
    pthread_cancel(tid);

    while(1)
    {
        printf("I am a main thread\n");
        sleep(1);
    }
    return 0;
}
