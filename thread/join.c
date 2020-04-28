#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thr_entry(void *arg)
{
    sleep(5);
    char *ptr = "nihao~~";
    pthread_exit(ptr);
    while(1)
    {
        printf("i am commom thread------%s\n", (char*)arg);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t mtid;
    // pthread_t pthread_self(void);
    // 获取调用线程的线程ID
    mtid = pthread_self();

    // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void*(start_routine)(void*), void *arg);
    pthread_t tid;
    char *param = "this is input param";
    int ret = pthread_create(&tid, NULL, thr_entry, (void*)mtid);
    if(ret != 0)
    {
        printf("pthread create error\n");
        return -1;
    }
    pthread_detach(tid);
    void *retval;
    ret = pthread_join(tid, &retval);
    printf("ret:%d------tid:%p------retval:%s\n", ret, tid, retval);
    while(1)
    {
        printf("i am main thread------\n");
        sleep(1);
    }
    
    return 0;
}
