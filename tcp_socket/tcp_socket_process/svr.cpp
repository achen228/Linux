#include "tcpsvr.hpp"
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void sigcb(int signo)
{
    //等待子进程
    (void)signo;
    while(1)
    {
        waitpid(-1, NULL, WNOHANG);
    }
}

int main()
{
    signal(SIGCHLD, sigcb); 

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
        TcpSvr ts2;
        struct sockaddr_in cliaddr;
        if(!ts.Accept(ts2, &cliaddr))
        {
            continue;
        }
        std::cout << "Server have a new connect, " << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << std::endl; 
        
        int pid = fork();
        if(pid < 0)
        {
            std::cout << "Create fork failed" << std::endl;
            exit(1);
        }
        else if(pid == 0)
        {
            //child
            while(1)
            {
                std::string msg;
                if(!ts2.Recv(msg))
                {
                    break;
                }
                std::cout << "cilent say:" << msg << std::endl;

                std::cout << "server say:";
                fflush(stdout);
                std::cin >> msg;
                ts2.Send(msg);
            }
            ts2.Close();
            exit(1);
        }
        else 
        {
            //father
            ts2.Close();
        }
    }

    ts.Close();
    return 0;
}
