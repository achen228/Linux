#include "tcpsvr.hpp"

void* ThreadStart(void* arg)
{
    pthread_detach(pthread_self());
    TcpSvr* ts = (TcpSvr*)arg;
    while(1)
    {
        std::string msg;
        ts->Recv(msg);
        std::cout << "client say:" << msg << std::endl;

        std::cout << "server say:";
        fflush(stdout);
        std::cin >> msg;
        ts->Send(msg);
    }
    ts->Close();
    delete ts;
}

int main()
{
    TcpSvr ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    if(!ts.Bind())
    {
        return 0;
    }

    if(!ts.Listen())
    {
        return 0;
    }

    while(1)
    {
        TcpSvr* ts2 = new TcpSvr();
        struct sockaddr_in cliaddr;
        if(!ts.Accept(ts2, &cliaddr))
        {
            continue;
        }
        std::cout << "Server have a new conncet, " << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << std::endl;
        
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, ThreadStart, (void*)ts2);
        if(ret < 0)
        {
            std::cout << "Create thread failed" << std::endl;
            return 0;
        }
    }
    
    ts.Close();
    return 0;
}
