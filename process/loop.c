#include <stdio.h>
#include <unistd.h>

int main()
{
    //pid_t getpid(void);
    //获取调用进程ID
    pid_t pid = getpid();
    while(1)
    {
        printf("hello world! %d\n", pid);
        sleep(2);
    }
    return 0;
}
