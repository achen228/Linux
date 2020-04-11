#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

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

        //需要将整体字符串解析出:程序名称+参数
        //"ls -a -l" -> "ls" "-a" "-l"
        char *ptr = tmp;
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
            execvp(argv[0], argv);
            //若子进程程序替换失败，则直接退出，因为终端不需要多个shell
            exit(0);
        }
        //等待子进程退出，避免产生僵尸进程
        wait(NULL);
    }
    return 0;
}
