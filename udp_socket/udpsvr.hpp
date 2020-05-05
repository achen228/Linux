#pragma once 
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

#define PORT 17777

class UdpSvr
{
    public:
        UdpSvr()
        {
            sock_ = -1;
        }

        ~UdpSvr()
        {}

        bool Create()
        {
            sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

        bool Send(std::string& msg, struct sockaddr_in* peeraddr)
        {

            ssize_t send_size = sendto(sock_, msg.c_str(), msg.size(), 0, (struct sockaddr*)peeraddr, sizeof(struct sockaddr_in));
            if(send_size < 0)
            {
                std::cout << "Send failed" << std::endl;
                return false;
            }
            return true;
        }

        bool Recv(std::string& msg, struct sockaddr_in* peeraddr)
        {
            char buf[1024] = {0};
            socklen_t peeraddr_len = sizeof(struct sockaddr_in);
            ssize_t recv_size = recvfrom(sock_, &buf, sizeof(buf) - 1, 0, (struct sockaddr*)peeraddr, &peeraddr_len);
            if(recv_size < 0)
            {
                std::cout << "Recv failed" << std::endl;
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
