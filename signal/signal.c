#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
    printf("receive a signo:%d\n", signo);
}

int main()
{
    //signal(SIGINT, SIG_IGN);
    
    struct sigaction new_act, old_act;
    new_act.sa_flags = 0;
    new_act.sa_handler = sigcb;

    // int sigemptyset(sigset_t *set);
    // 清空set信号集合
    sigemptyset(&new_act.sa_mask);

    // sigaction使用newact替换2号信号的处理动作，将原有动作保存到oldact中
    sigaction(2, &new_act, &old_act);

    while(1)
    {
        printf("xuexihaolei\n");
        sleep(10);
    }

    return 0;
}
