#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("hello world!\n");
    //int execl(const char *path, const char *arg, ...);
    //使用path这个路径的程序，替换当前进程要运行的程序
    //让当前进程运行ls这个程序的功能
    //后边arg以及...都是这个程序的运行参数
    execl("/usr/bin/ls", "ls", "-l", "-a", NULL);

    //int execlp(const char *file, const char *arg, ...);
    //execlp("pwd", "pwd", NULL);

    //int execv(const char *path, char *const argv[]);
    //char *argv[32] = {NULL};
    //argv[0] = "ls";
    //argv[1] = "-l";
    //argv[2] = "-a";
    //argv[3] = NULL;
    //execv("usr/bin/ls", argv);

    //int execle(const char *path, const char *arg, ..., char *const envp[]);
    //execle("/usr/bin/ls", "ls", "-l", NULL, NULL);

    printf("world hello!\n");
    return 0;
}
