#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *env[])
{
    //char *getenv(const char *name); 
    //通过环境变量名称获取内容 
    //char *ptr = getenv("PWD"); 
    //printf("ptr:%s\n", ptr); 
    int i;
    for (i = 0; env[i] != NULL; i++)
    {
        printf("env[%d] = [%s]\n", i , env[i]);
    }
    return 0;
}
