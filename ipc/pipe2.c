#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    // ls | grep Make
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
        // ls
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        execlp("ls", "ls", NULL);
        exit(0);
    }
    int pid2 = fork();
    if (pid2 == 0)
    {
        // grep Make
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        execlp("grep", "grep", "Make", NULL);
        exit(0);
    }
    close(pipefd[1]);
    close(pipefd[0]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    return 0;
}
