#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TcpSvr
{
public:
    TcpSvr()
    {
        _sockfd = -1;
    }

    ~TcpSvr()
    {}

    //创建套接字
    bool CreateSocket()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(_sockfd < 0)
        {
            perror("socket failed");
            return false;
        }

        int i = 1;
        setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
        return true;
    }

    //绑定地址信息
    bool Bind(const std::string& ip, uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind failed");
            return false;
        }
        return true;
    }

    //侦听
    bool Listen(int backlog = 5)
    {
        int ret = listen(_sockfd, backlog);
        if(ret < 0)
        {
            perror("listen failed");
            return false;
        }
        return true;
    }

    //获取连接
    //peeraddr:出参，返回客户端的地址信息
    //ts:返回一个TcpSvr类的实例化指针，在这个类的实例化指针当中保存新创建出来的套接字描述符，上层调用者可以使用返回的类的实例化指针和客户端进行通信
    bool Accept(struct sockaddr_in* peeraddr, TcpSvr* ts)
    {
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int svrfd = accept(_sockfd, (struct sockaddr*)peeraddr, &addrlen);
        if(svrfd < 0)
        {
            perror("accept failed");
            return false;
        }
        ts->_sockfd = svrfd;
        return true;
    }

    //发起连接(客户端)
    bool Connect(std::string& ip, uint16_t port)
    {
        struct sockaddr_in destaddr;
        destaddr.sin_family = AF_INET;
        destaddr.sin_port = htons(port);
        destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
        int ret = connect(_sockfd, (struct sockaddr*)&destaddr, sizeof(destaddr));
        if(ret < 0)
        {
            perror("connect failed");
            return false;
        }
        return true;
    }

    //发送数据
    bool Send(std::string& msg)
    {
        int sendsize = send(_sockfd, msg.c_str(), msg.size(), 0);
        if(sendsize < 0)
        {
            perror("send failed");
            return false;
        }
        return true;
    }

    //接收数据
    //data:是一个出参，将收到的数据返回给上层调用者
    bool Recv(std::string* data)
    {
        char buf[1024] = {0};
        int recvsize = recv(_sockfd, buf, sizeof(buf) - 1, 0);
        if(recvsize < 0)
        {
            perror("recv failed");
            return false;
        }
        else if(recvsize == 0)
        {
            printf("peeraddr shutdown connect\n");
            return false;
        }
        (*data).assign(buf, recvsize);
        return true;
    }

    //关闭套接字
    void Close()
    {
        close(_sockfd);
        _sockfd = -1;
    }

    void SetFd(int fd)
    {
        _sockfd = fd;
    }

    int GetFd()
    {
        return _sockfd;
    }
private:
    int _sockfd;
};
