#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <vector>
#include "TcpSvr.hpp"

class SelectSvr
{
public:
    SelectSvr()
    {
        _maxfd = -1;
        FD_ZERO(&_readfds);
    }

    ~SelectSvr()
    {}

    void AddFd(int fd)
    {
        //1.添加
        FD_SET(fd, &_readfds);
        //2.更新最大文件描述符
        if(fd > _maxfd)
        {
            _maxfd = fd;
        }
    }

    void DeleteFd(int fd)
    {
        //1.删除
        FD_CLR(fd, &_readfds);
        //2.更新最大文件描述符
        //从后向前遍历，找到第一个文件描述符还在可读事件集合当中的，则它就是要找的最大文件描述符
        for(int i = _maxfd; i >= 0; i--)
        {
            if(FD_ISSET(i, &_readfds) == 0)
                continue;
            _maxfd = i;
            break;
        }
    }

    bool SelectWait(std::vector<TcpSvr>* vec)
    {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 3000;

        fd_set tmp = _readfds;
        int ret = select(_maxfd + 1, &tmp, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("select failed");
            return false;
        }
        else if(ret == 0)
        {
            //等待超时
            return false;
        }

        //正常情况
        for(int i = 0; i <= _maxfd; i++)
        {
            if(FD_ISSET(i, &tmp))
            {
                //返回就绪的文件描述符i,返回tcp类的对象
                TcpSvr ts;
                ts.SetFd(i);
                vec->push_back(ts);
            }
        }
        return true;
    }
private:
    int _maxfd; //文件描述符的数量
    fd_set _readfds; //可读事件集合
};
