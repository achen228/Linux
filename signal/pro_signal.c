#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    // int kill(pid_t pid, int sig);
    // 向pid进程发送sig信号
    //kill(getpid(), SIGKILL);
    
    // int raise(int sig);
    // 向自己发送sig信号
    //raise(SIGTERM);
    
    // void abort(void);
    // 给自己发送SIGABRT信号
    //abort();
    
    // unsigned int alarm(unsigned int seconds);
    // 经过seconds秒之后，给自己发送一个SIGALRM信号-->定时器
    alarm(3);

    while(1)
    {
        printf("hello world\n");
        sleep(1);
    }
    return 0;
}
