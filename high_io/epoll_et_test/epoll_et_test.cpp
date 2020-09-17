#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string>

int main()
{
    //设置文件描述性属性为非阻塞
    int flag = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, flag | O_NONBLOCK);

    //创建epoll操作句柄
    int epollfd = epoll_create(10);
    if(epollfd < 0)
    {
        perror("epoll_create failed");
        return 0;
    }

    //添加文件描述符
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = 0;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev);
    if(ret < 0)
    {
        perror("epoll_ctl failed");
        return 0;
    }

    //监控
    while(1)
    {
        struct epoll_event fd_arr[10];
        ret = epoll_wait(epollfd, fd_arr, sizeof(fd_arr)/sizeof(fd_arr[0]), -1);
        if(ret < 0)
        {
            perror("epoll_wait failed");
            return 0;
        }

        for(int i = 0; i < ret; i++)
        {
            if(fd_arr[i].data.fd == 0)
            {
                std::string read_data;
                while(1)
                {
                    char buf[3] = {0};
                    ssize_t readsize = read(0, buf, sizeof(buf) - 1);
                    if(readsize < 0)
                    {
                        if(errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            //说明数据读取完了
                            goto myend;
                        }
                        perror("read failed");
                        return 0;
                    }

                    read_data += buf;
                    if(readsize < (ssize_t)sizeof(buf) - 1)
                    {
myend:
                        printf("read buf is %s\n", read_data.c_str());
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
