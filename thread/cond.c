// 这个demo实现条件变量的使用
// 以自己吃面和厨师做面为例子：
//      自己想要吃面，前提是有面，没有面则等待
//      厨师做面，做好面后，唤醒吃面的人

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int _have_noodle = 0;
pthread_mutex_t mutex; // 定义互斥锁
pthread_cond_t cond_eat; // 定义条件变量
pthread_cond_t cond_cook; // 定义条件变量

void *eat_noodle(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(_have_noodle == 0)
        {
            // 没有面不能吃
            // pthread_cond_wait实现了三步操作：
            // 1.解锁
            // 2.休眠
            // 3.被唤醒后加锁
            // 其中解锁和休眠操作是原子操作
            pthread_cond_wait(&cond_eat, &mutex);
        }
        // 能走下来表示_have_noodle == 1; 表示有面
        printf("eat noodle, delicious\n");
        _have_noodle = 0;
        pthread_cond_signal(&cond_cook);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *cook_noodle(void *arg)
{
   while(1)
   {
        pthread_mutex_lock(&mutex);
        while(_have_noodle == 1)
        {
            //现在有面，但是没有人吃，不能继续做
            pthread_cond_wait(&cond_cook, &mutex);
        }
        // 能走下来表示_have_noodle == 1; 表示没有面
        printf("cook noodle, come on\n");
        _have_noodle = 1;
        pthread_cond_signal(&cond_eat);
        pthread_mutex_unlock(&mutex);
   }

   return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_eat, NULL);
    pthread_cond_init(&cond_cook, NULL);

    int i;
    for(i = 0; i < 4; i++)
    {

        int ret = pthread_create(&tid1, NULL, eat_noodle, NULL);
        if(ret != 0)
        {
            printf("pthread create error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++)
    {
        int ret = pthread_create(&tid2, NULL, cook_noodle, NULL);
        if(ret != 0)
        {
            printf("pthread create error\n");
            return -1;
        }
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_eat);
    pthread_cond_destroy(&cond_cook);

    return 0;
}
