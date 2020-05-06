#include "udpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "./cli [ip]" << std::endl;
        return 0;
    }
    std::string ip = argv[1];

    UdpSvr us;
    if(!us.Create())
    {
        return 0;
    }

    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(PORT);
    svraddr.sin_addr.s_addr = inet_addr(ip.c_str());

    while(1)
    {
        std::string msg;
        std::cout << "client say:";
        fflush(stdout);
        std::cin >> msg;
        if(!us.Send(msg, &svraddr))
        {
            continue;
        }

        if(!us.Recv(msg, &svraddr))
        {
            continue;
        }
        std::cout << "server say:" << msg << std::endl;
    }
    us.Close();
    return 0;
}
