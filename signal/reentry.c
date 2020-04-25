#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int a = 1;
int b = 1;

int sum(int *a, int *b)
{
    (*a)++;
    sleep(3);
    (*b)++;
    return *a + *b;
}

void sigcb(int no)
{    
    printf("signal---------%d\n", sum(&a, &b));
}

int main()
{
    signal(SIGINT, sigcb);
    printf("main---------%d\n", sum(&a, &b));
    return 0;
}
