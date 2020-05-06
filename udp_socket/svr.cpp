#include "udpsvr.hpp"

int main()
{
    UdpSvr us;
    if(!us.Create())
    {
        return 0;
    }

    if(!us.Bind())
    {
        return 0;
    }

    while(1)
    {
        std::string msg;
        struct sockaddr_in cliaddr;
        if(!us.Recv(msg, &cliaddr))
        {
            continue;
        }
        std::cout << "client say:" << msg << std::endl;

        std::cout << "server say:";
        fflush(stdout);
        std::cin >> msg;
        if(!us.Send(msg, &cliaddr))
        {
            continue;
        }
    }
    us.Close();
    return 0;
}
