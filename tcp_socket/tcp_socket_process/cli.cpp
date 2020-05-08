#include "tcpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "./cli [ip]" << std::endl;
        return 0;
    }
    std::string ip = argv[1];

    TcpSvr ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    struct sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(PORT);
    svraddr.sin_addr.s_addr = inet_addr(ip.c_str());
    if(!ts.Connect(&svraddr))
    {
        return 0;
    }

    while(1)
    {
        std::string msg;
        std::cout << "client say:";
        fflush(stdout);
        std::cin >> msg;
        ts.Send(msg);

        ts.Recv(msg);
        std::cout << "server say:" << msg << std::endl;
    }

    ts.Close();
    return 0;
}
