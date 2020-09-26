#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
    printf("receive a signo:%d\n", signo);
}

int main()
{
    //将SIGINT信号设置为忽略处理
    signal(SIGINT, SIG_IGN);
    
    struct sigaction new_act, old_act;
    new_act.sa_flags = 0;
    new_act.sa_handler = sigcb;

    // int sigemptyset(sigset_t *set);
    // 清空set信号集合
    sigemptyset(&new_act.sa_mask);

    // sigaction使用newact替换SIGINT信号的处理动作，将原有动作保存到oldact中
    sigaction(SIGINT, &new_act, &old_act);

    while(1)
    {
        printf("hello world\n");
        sleep(1);
    }

    return 0;
}
