#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
    printf("receive signo:%d\n", signo);
}

int main()
{
    //自定义函数处理SIGINT信号，非可靠信号
    signal(SIGINT, sigcb);
    //自定义函数处理SIGRTMIN+4信号，可靠信号
    signal(SIGRTMIN+4, sigcb);

    sigset_t set;
    sigemptyset(&set); //清空set集合

    sigfillset(&set); //将所有信号都添加到set集合中

    sigprocmask(SIG_BLOCK, &set, NULL); //阻塞set集合中的信号

    printf("press enter continue\n");
    getchar(); //按下回车键后继续向下运行

    sigprocmask(SIG_UNBLOCK, &set, NULL); //将set集合中的信号解除阻塞

    while(1)
    {
        sleep(1);
    }
    return 0;
}
