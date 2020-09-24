#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pid = fork();
    if (pid < 0)
    {
        //error是一个全局变量，存储每次系统调用出现错误原因编号
        //strerror，通过错误编号获取字符串错误原因
        printf("fork error:%s\n", strerror(errno));
        //perror直接打印上一个系统调用错误原因
        perror("fork error\n");
    }
    else if (pid == 0)
    {
        sleep(3);
        exit(1);
    }
    //pid_t wait(int *status);
    //阻塞等待任意一个子进程退出，获取返回值
    //wait(NULL);

    //pid_t waitpid(pid_t pid, int *status, int options);
    //阻塞等待任意一个子进程或者指定的子进程退出
    //pid = -1:等待任意一个子进程;  pid > 0:等待指定子进程
    //options:WNOHANG，将waitpid设置为非阻塞  0:默认阻塞
    //返回值:若WNOHANG被指定，没有子进程退出则立即报错返回0;错误为-1;
    
    //pid>0,此时的pid是子进程的pid，因为父进程返回的是子进程的pid
    //也就是waitpid函数指定等待子进程退出
    int status;
    while (waitpid(pid, &status, WNOHANG) == 0)
    {
        //非阻塞轮询操作
        printf("drink coffee\n");
        sleep(1);
    }

    if ((status & 0x7f) == 0)
    {
        printf("exit code:%d\n", (status >> 8) & 0xff);
    }

    //子进程正常退出，返回非0
    if (WIFEXITED (status))
    {
        //获取子进程的退出码
        printf("exit code:%d\n", WEXITSTATUS(status));
    }

    while(1)
    {
        printf("---parent---\n");
        sleep(1);
    }

    return 0;
}
