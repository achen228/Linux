//这个demo体会条件变量的使用
//多个厨师，多个顾客，一个碗

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex; //互斥锁保证对临界资源的唯一访问性
pthread_cond_t cook_cond; //条件变量保证对临界资源访问的时序合理性
pthread_cond_t customer_cond; //条件变量保证对临界资源访问的时序合理性

int bowl = 0; //初始0表示碗中没有面，需要厨师去做

void* thr_cook(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(bowl != 0)
        {
            //表示碗中有面，则厨师陷入等待，等待前必须解锁，被唤醒后加锁
            // pthread_cond_wait函数完成了解锁、等待、加锁的三步过程，其中解锁和等待一步完成，保证原子操作
            pthread_cond_wait(&cook_cond, &mutex);
        }
        //表示碗中没有面，需要厨师去做
        printf("I make a bowl noodle\n");
        bowl = 1;
        pthread_mutex_unlock(&mutex);
        //通知顾客面做好了，可以吃
        pthread_cond_signal(&customer_cond);
    }
    return NULL;
}

void* thr_customer(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(bowl == 0)
        {
            //表示碗中没有面，则顾客陷入等待，等待前必须解锁，被唤醒后加锁
            pthread_cond_wait(&customer_cond, &mutex);
        }
        //表示碗中有面，顾客可以吃
        printf("I eat a bowl noodle, delicious\n");
        bowl = 0;
        pthread_mutex_unlock(&mutex);
        //通知厨师面吃完了，需要再做
        pthread_cond_signal(&cook_cond);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cook_cond, NULL);
    pthread_cond_init(&customer_cond, NULL);

    pthread_t cook_tid[4], customer_tid[4];
    int ret, i;
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&cook_tid[i], NULL, thr_cook, NULL);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }

        ret = pthread_create(&customer_tid[i], NULL, thr_customer, NULL);
        if(ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }
    }

    for(i = 0; i < 4; i++)
    {
        pthread_join(cook_tid[i], NULL);
        pthread_join(customer_tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cook_cond);
    pthread_cond_destroy(&customer_cond);
    return 0;
}
