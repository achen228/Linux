#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

#include "TcpSvr.hpp"

class EpollSvr
{
public:
    EpollSvr()
    {
        _epollfd = epoll_create(10);
        if(_epollfd < 0)
        {
            exit(1);
        }
    }

    ~EpollSvr()
    {}

    //对于ET模式，需要判断一下是否开启ET模式
    bool AddFd(int fd, bool is_et = false)
    {
        struct epoll_event ev;
        if(is_et)
        {
            ev.events = EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLIN;
        }
        ev.data.fd = fd;

        int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);
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
        int ret = epoll_wait(_epollfd, fd_arr, sizeof(fd_arr)/sizeof(fd_arr[0]), -1);
        if(ret < 0)
        {
            perror("epoll_wait failed");
            return false;
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
