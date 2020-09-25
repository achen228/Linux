#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    // ls -l | grep pipe.c
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret < 0)
    {
        perror("pipe error");
        return -1;
    }

    int pid1 = fork();
    if (pid1 == 0)
    {
        // child
        //将标准输出重定向到管道写入端，向1写入数据就相当于向管道写入数据
        dup2(pipefd[1], 1);
        execlp("ls", "ls", "-l", NULL);
        exit(0);
    }

    int pid2 = fork();
    if (pid2 == 0)
    {
        // grep pipe.c
        // 关闭写端，ls -l进程退出后，所有写端就会被关闭
        close(pipefd[1]);
        //将标准输入重定向到管道读端，从0读取数据就相当于从管道读取数据
        dup2(pipefd[0], 0);
        execlp("grep", "grep", "pipe.c", NULL);
        exit(0);
    }

    //关闭父进程的读写端
    close(pipefd[1]);
    close(pipefd[0]);
    // 等待子进程退出
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    return 0;
}
