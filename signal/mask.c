#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
    printf("receive signo:%d\n", signo);
}

int main()
{
    signal(SIGINT, sigcb);
    signal(40, sigcb);

    // 实现信号的阻塞
    // 先将所有信号全部阻塞
    // getchar(); 在用户按下回车之前，这些信号一直被阻塞
    // 对所有信号解除阻塞--这时候信号解除阻塞将被处理

    // int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    // how:
    //     SIG_BLOCK    向阻塞集合中加入set集合中的信号 block = mask | set
    //     SIG_UNBLOCK    从阻塞集合中移除set集合中的信号 block = mask & (~set)
    //     SIG_SETMASK    将set集合的信号设置为阻塞集合 block = set
    // set：要阻塞/解除阻塞的信号集合                                                            
    // oldset：用于保存修改前阻塞集合中的信号
    
    sigset_t set, oldset;
    sigemptyset(&set);

    // int sigfillset(sigset_t *set);
    // 向set集合中添加所有信号
    // int sigaddset(sigset_t *set, int signum);
    // 向set集合中添加signum集合
    sigfillset(&set);

    // 阻塞set中的信号
    sigprocmask(SIG_BLOCK, &set, &oldset);

    getchar();

    sigprocmask(SIG_UNBLOCK, &set, NULL);
    //sigprocmask(SIG_SETMASK, &oldset, NULL);

    return 0;
}
