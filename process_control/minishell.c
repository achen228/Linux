//这个demo演示shell的简单使用
//1.等待标准输入
//2.获取标准输入后，进行字符串解析，获取命令名称和命令参数
//3.创建子进程，进行程序替换
//4.等待子进程退出


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    while(1)
    {
        printf("[zhangchen@minishell ~]$ ");
        fflush(stdout);

        char buf[1024] = {0};
        fgets(buf, sizeof(buf) - 1, stdin);
        buf[strlen(buf) - 1] = '\0'; //将用户输入的最后一个换行符改为 '\0'

        char* ptr = buf;
        char* argv[] = {0};
        int argc = 0;
        while(*ptr != '\0')
        {
            if(*ptr != ' ')
            {
                argv[argc] = ptr;
                argc++;
                while(*ptr != ' ' && *ptr != '\0')
                {
                    ptr++;
                }
                *ptr = '\0';
                ptr++;
                continue;
            }
            ptr++;
        }
        argv[argc] = NULL; //最后一个参数的下一个位置置NULL

        pid_t pid = fork();
        if(pid == 0)
        {
            //子进程
            int ret = execvp(argv[0], argv);
            if(ret < 0)
            {
                perror("execvp error");
                return -1;
            }
            exit(1);
        }

        //父进程等待子进程退出，避免产生僵尸进程
        wait(NULL);
    }
    return 0;
}
