//简单使用select
//1.准备需要监控的文件描述符
//2.将文件描述符添加到对应的事件集合中
//3.监控
//4.判断文件描述符是否在集合中
//5.处理文件描述符

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
begin:
    //监控0号文件描述符的可读事件
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    //阻塞监控
    //int ret = select(0 + 1, &readfds, NULL, NULL, NULL);
    //带有超时时间的监控
    int ret = select(0 + 1, &readfds, NULL, NULL, &tv);
    if(ret < 0)
    {
        //监控出错
        perror("select failed");
        return 0;
    }
    else if(ret == 0)
    {
        //等待超时
        printf("select timeout\n");
        //循环监控，返回的集合当中只返回就绪的文件描述符
        ret = FD_ISSET(0, &readfds);
        if(ret == 0)
        {
            //理论上是正确的
            printf("0 fd is not in readfds\n");
            goto begin;
        }
        else if(ret != 0)
        {
            printf("0 fd is in readfds\n");
            return 0;
        }
    }

    //ret > 0, 返回就绪的文件描述符的个数
    printf("ret:%d\n", ret);

    //判断0号文件描述符是否在可读事件集合中
    if(FD_ISSET(0, &readfds) != 0)
    {
        char buf[1024] = {0};
        read(0, buf, sizeof(buf) - 1);
        printf("buf:%s\n", buf);
    }

    //将0号文件描述符从可读事件集合中删除
    FD_CLR(0, &readfds);
    return 0;
}
