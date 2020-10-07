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
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    umask(0);
    while(1)
    {
        printf("[zhangchen@minishell ~]$ ");
        fflush(stdout);

        char buf[1024] = {0};
        fgets(buf, sizeof(buf) - 1, stdin);
        buf[strlen(buf) - 1] = '\0'; //将用户输入的最后一个换行符改为 '\0'

        //文件描述符的重定向
        char* ptr = buf;
        int flag =  0;
        char* pfile = NULL;
        while(*ptr != '\0')
        {
            if(*ptr == '>')
            {
                //这是清空重定向
                flag = 1;
                //将>替换成结尾标志，则后面的命令解析到此位置就完毕了
                *ptr = '\0';
                ptr++;
                //有第二个>则是追加重定向
                if(*ptr == '>')
                {
                    flag = 2;
                    ptr++;
                }

                //将文件.txt之前的空格走完
                while(*ptr == ' ' && *ptr != '\0')
                {
                    ptr++;
                }
                //将pfile这个指针指向了.txt文件中的开始位置
                pfile = ptr;
                //将.txt文件字符走完
                while(*ptr != ' ' && *ptr != '\0')
                {
                    ptr++;
                }
                *ptr = '\0';
            }
            ptr++;
        }

        ptr = buf;
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
            //清空重定向
            if(flag == 1)
            {
                int fd = open(pfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
                //将标准输入重定向到pfile，原本要打印的数据就会被写入文件中
                dup2(fd, 1);
            }
            //追加重定向
            else if(flag == 2)
            {
                int fd = open(pfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
                //将标准输入重定向到pfile，原本要打印的数据就会被写入文件中
                dup2(fd, 1);
            }

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
