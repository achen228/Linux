#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

int main()
{
    while (1)
    {
        printf("[achen@loacl]$ ");
        fflush(stdout);
        char tmp[1024] = {0};
        scanf("%[^\n]%*c", tmp);  //获取一个连续的字符串
        // %[^\n]:从标准输入缓冲区取数据直到遇到\n截至
        // %*c:将缓冲区中剩下的\n取出来；否则造成死循环
        printf("[%s]\n", tmp);

        //ls -l >> test.txt
        char *ptr = tmp;
        int redirect_flag = 0;
        char *redirect_file = NULL;
        while(*ptr != '\0')
        {
            if(*ptr == '>')
            {
                *ptr = '\0';
                ptr++;
                redirect_flag = 1; // 清空重定向
                if(*(ptr++) == '>')
                {
                    *ptr = '\0';
                    ptr++;
                    redirect_flag = 2; // 追加重定向
                }
                while(isspace(*ptr) && *ptr != '\0')
                    ptr++;
                redirect_file = ptr;
                while(!isspace(*ptr) && *ptr != '\0')
                    ptr++;
                *ptr = '\0';
            }
            ptr++;
        }

        //需要将整体字符串解析出:程序名称+参数
        //"ls -a -l" -> "ls" "-a" "-l"
        ptr = tmp;
        int argc = 0;
        char *argv[32] = {NULL};
        while (*ptr != '\0')
        {
            if(!isspace(*ptr))
            {
                //指针走到非空白字符处
                argv[argc] = ptr;
                argc++;
                while (!isspace(*ptr) && *ptr != '\0')
                    ptr++;
                *ptr = '\0';
                ptr++;
                continue;
            }
            ptr++;
        }
        int i;
        for ( i = 0; i < argc; i++  )
        {
            printf("argv[%d] = [%s]\n", i, argv[i]);                   
        }
        argv[argc] = NULL;

        if (!strcmp(argv[0], "cd"))
        {
            //改变当前工作路径
            chdir(argv[1]);
            continue;
        }

        int pid = fork();
        if (pid == 0)
        {
            int fd = 1;
            if(redirect_flag == 1)
            {
                fd = open(redirect_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
            }
            else if(redirect_flag == 2)
            {
                fd = open(redirect_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
            }
            // 标准输出重定向，将原本要写入标准输出1的数据写入到新文件中
            dup2(fd, 1);
            execvp(argv[0], argv);
            //若子进程程序替换失败，则直接退出，因为终端不需要多个shell
            exit(0);
        }
        //等待子进程退出，避免产生僵尸进程
        wait(NULL);
    }
    return 0;
}
