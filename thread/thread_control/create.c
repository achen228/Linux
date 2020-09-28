#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* thr_start(void* arg)
{
    while(1)
    {
        printf("I am a normal thread:%s\n", (char*)arg);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    char* str = "hello world";
    int ret = pthread_create(&tid, NULL, thr_start, (void*)str);
    if(ret != 0)
    {
        printf("pthread_create failed\n");
        return -1;
    }
    printf("tid:%lu\n", tid);

    while(1)
    {
        printf("I am a main thread\n");
        sleep(1);
    }
    return 0;
}
