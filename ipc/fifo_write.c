#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main()
{
    // int mkfifo(const char *pathname, mode_t  mode);
    // pathname:管道文件名称
    // mode:管道文件权限
    // 返回值：0  失败：-1
    char *fifo = "./test.fifo";
    umask(0);
    int ret = mkfifo(fifo, 0664); 
    if (ret < 0)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo error");
            return -1;
        }
    }

    printf("start open --------\n");
    int fd = open(fifo, O_WRONLY);
    printf("end open --------\n");
    if (fd < 0)
    {
        perror("open error");
        return -1;
    }
    printf("fifo:%s open success!\n", fifo);
   
    while (1)
    {
        char buf[1024] = {0};
        printf("I say:");
        fflush(stdout);
        scanf("%s,", buf);
        write(fd, buf, strlen(buf));
    }
    close(fd);
    return 0;
}
