#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    //1.定义事件结构数组
    struct pollfd fd_arr[10];
    fd_arr[0].fd = 0;
    fd_arr[0].events = POLLIN;
    //2.监控
    while(1)
    {
        int ret = poll(fd_arr, 1, 1000);
        if(ret < 0)
        {
            perror("poll failed");
            return -1;
        }
        else if(ret == 0)
        {
            printf("timeout\n");
            continue;
        }

        //3.当产生可读事件的时候，处理事件
        for(int i = 0; i < ret; i++)
        {
            if(fd_arr[i].revents == POLLIN)
            {
                char buf[1024] = {0};
                read(fd_arr[i].fd, buf, sizeof(buf) - 1);
                printf("read content is [%s]\n", buf);
            }
        }
    }
    return 0;
}
