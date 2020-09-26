//这个demo来演示函数的可重入与不可重入

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int a = 1;
int b = 1;

int sum(int* a, int* b)
{
    (*a)++;
    sleep(3);
    (*b)++;
    return *a + *b;
}

void sigcb(int signo)
{    
    (void)signo;
    printf("signal---------%d\n", sum(&a, &b));
}

int main()
{
    //当用户输入ctrl + c时，会用sigcb函数替换当前信号的执行
    signal(SIGINT, sigcb);
    printf("main---------%d\n", sum(&a, &b));
    return 0;
}
