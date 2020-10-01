#include "threadpool.hpp"

void test1_func(int data)
{
    int sec = (data % 3) + 1;
    printf("tid:%lup --- get data:%d --- sleep:%d\n", pthread_self(), data, sec);
    sleep(sec);
}

void test2_func(int data)
{
    printf("tid:%lup --- get data:%d --- test2_func\n", pthread_self(), data);
    sleep(1);
}

int main()
{
    ThreadPool pool;
    for(int i = 0; i < 10; i++)
    {
        ThreadTask task;
        if(i %2 == 0)
        {
            task.SetTask(i, test1_func);
        }
        else 
        {
            task.SetTask(i, test2_func);
        }
        pool.PushTask(task);
    }

    //让主线程休息暂时不要退出，方便观察线程中数据的处理效果
    sleep(1000);
    return 0;
}
