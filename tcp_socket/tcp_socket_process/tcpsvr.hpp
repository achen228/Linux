#pragma once 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define PORT 17778

class TcpSvr
{
    public:
        TcpSvr()
        {
            sock_ = -1;
        }

        ~TcpSvr()
        {}

        bool CreateSock()
        {
            sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(sock_ < 0)
            {
                std::cout << "Create failed" << std::endl;
                return false;
            }
            return true;
        }

        bool Bind()
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(PORT);
            addr.sin_addr.s_addr = inet_addr("0.0.0.0");
            int ret = bind(sock_, (struct sockaddr*)&addr, sizeof(addr));
            if(ret < 0)
            {
                std::cout << "Bind failed" << std::endl;
                return false;
            }
            return true;
        }

        bool Listen(int backlog = 5)
        {
            int ret = listen(sock_, backlog);
            if(ret < 0)
            {
                std::cout << "Listen failed" << std::endl;
                return false;
            }
            return true;
        }

        bool Accept(TcpSvr& ts, struct sockaddr_in* addr = NULL)
        {
            struct sockaddr_in cliaddr;
            socklen_t cliaddr_len = sizeof(cliaddr);
            int newsock = accept(sock_, (struct sockaddr*)&cliaddr, &cliaddr_len);
            if(newsock < 0)
            {
                std::cout << "Accept failed" << std::endl;
                return false;
            }
            ts.sock_ = newsock;

            if(addr != NULL)
            {
                memcpy(addr, &cliaddr, cliaddr_len);
            }
            return true;
        }

        bool Connect(struct sockaddr_in* svraddr)
        {
            int ret = connect(sock_, (struct sockaddr*)svraddr, sizeof(struct sockaddr_in));
            if(ret < 0)
            {
                std::cout << "Connect failed" << std::endl;
                return false;
            }
            return true;
        }

        bool Send(std::string& msg)
        {
           ssize_t send_size = send(sock_, msg.c_str(), msg.size(), 0);
           if(send_size < 0)
           {
                std::cout << "Send failed" << std::endl;
                return false;
           }
           return true;
        }

        bool Recv(std::string& msg)
        {
            char buf[1024] = {0};
            ssize_t recv_size = recv(sock_, buf, sizeof(buf) - 1, 0);
            if(recv_size < 0)
            {
                std::cout << "Recv failed" << std::endl;
                return false;
            }
            else if(recv_size == 0)
            {
                std::cout << "Peer close connect" << std::endl;
                return false;
            }
            msg.assign(buf, recv_size);
            return true;
        }

        void Close()
        {
            close(sock_);
            sock_ = -1;
        }
    private:
        int sock_;
};
