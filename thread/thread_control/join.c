#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thr_start(void* arg)
{
    while(1)
    {
        printf("I am a nurmal thread:%d\n", (*(int*)arg));
        sleep(1);
        char* retval = "我是线程返回值";
        pthread_exit(retval);
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

    void* retval;
    pthread_join(tid, &retval);
    printf("retval:%s\n", (char*)retval);

    while(1)
    {
        printf("I am a main thread\n");
        sleep(1);
    }
    return 0;
}
