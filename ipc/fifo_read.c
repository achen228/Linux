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
    const char* fifo = "./test.fifo";
    umask(0);
    int ret = mkfifo(fifo, 0664); 
    if (ret < 0 && errno != EEXIST)
    {
        perror("mkfifo error");
        return -1;
    }

    int fd = open(fifo, O_RDONLY);
    if (fd < 0)
    {
        perror("open fifo error");
        return -1;
    }
    printf("open fifo success\n");

    while (1)
    {
        char buf[1024] = {0};
        int ret = read(fd, buf, sizeof(buf) - 1);
        if(ret < 0)
        {
            perror("read error");
            return -1;
        }
        else if(ret == 0)
        {
            printf("all write is closed\n");
            return -1;
        }

        printf("read buf is:%s\n", buf);
    }
    close(fd);
    return 0;
}
