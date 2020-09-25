#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    // 管道必须创建于子进程之前（子进程这样才能复制到管道的操作句柄）
    // int pipe(int pipefd[2]);
    int pipefd[2];
    int ret = pipe(pipefd); 
    if (ret < 0)
    {
        perror("pipe error");
        return -1;
    }

    int pid = fork();
    if (pid == 0)
    {
        // child
        // 关闭子进程写端，但是父进程的写端没有关闭，子进程依然可以读取数据
        close(pipefd[1]);  
        char buf[1024] = {0};
        // 若所有写端被关闭，则read不再阻塞，读完数据后返回0
        // 此时父进程的写端没有被关闭，则缓冲区中没有数据的时候，read会阻塞等待
        int ret = read(pipefd[0], buf, sizeof(buf) - 1);
        printf("child read buf:[%d-%s]\n", ret, buf);
        // 子进程退出
        exit(0);
    }
    else if (pid > 0)
    {
        // parent
        // 让子进程先运行
        sleep(1);
        //关闭父进程读端，但是子进程的读端没有关闭，父进程依然可以写入数据
        close(pipefd[0]);
        // 若所有读端被关闭，则write会触发异常，导致进程退出
        // 此时子进程的读端没有被关闭，则write可以写入数据
        const char* str = "hello world";
        write(pipefd[1], str, strlen(str));
    }

    //等待子进程退出
    wait(NULL);
    printf("-------------------------\n");
    return 0;
}
