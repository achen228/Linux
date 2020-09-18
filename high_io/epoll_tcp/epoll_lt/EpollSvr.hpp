#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

#include "TcpSvr.hpp"

class EpollSvr
{
public:
    EpollSvr()
    {
        _epollfd = -1;
    }

    ~EpollSvr()
    {}

    bool Init()
    {
        _epollfd = epoll_create(10);
        if(_epollfd < 0)
        {
            return false;
        }
        return true;
    }

    bool AddFd(int fd)
    {
        struct epoll_event ep;
        ep.events = EPOLLIN;
        ep.data.fd = fd;
        int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ep);
        if(ret < 0)
        {
            perror("epoll_ctl failed");
            return false;
        }
        return true;
    }

    bool DeleteFd(int fd)
    {
        int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
        if(ret < 0)
        {
            perror("epoll_ctl failed");
            return false;
        }
        return true;
    }

    bool EpollWait(std::vector<TcpSvr>* out)
    {
        struct epoll_event fd_arr[10];
        int ret = epoll_wait(_epollfd, fd_arr, sizeof(fd_arr) / sizeof(fd_arr[0]), -1);
        if(ret < 0)
        {
            perror("epoll_wait failed");
            return false;
        }
        else if(ret == 0)
        {
            printf("epoll_wait timeout\n");
            return false;
        }

        //防止数组越界
        if(ret > int(sizeof(fd_arr) / sizeof(fd_arr[0])))
        {
            ret = sizeof(fd_arr) / sizeof(fd_arr[0]);
        }

        for(int i = 0; i < ret; i++)
        {
            TcpSvr ts;
            ts.SetFd(fd_arr[i].data.fd);
            out->push_back(ts);
        }
        return true;
    }
private:
    int _epollfd;
};
