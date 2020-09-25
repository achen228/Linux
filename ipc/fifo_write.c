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

    int fd = open(fifo, O_WRONLY);
    if (fd < 0)
    {
        perror("open fifo error");
        return -1;
    }
    printf("open fifo success\n");
   
    int i = 0;
    while (1)
    {
       char buf[1024] = {0};
       sprintf(buf, "hello world---%d", i++);
       write(fd, buf, strlen(buf));
       sleep(1);
    }
    close(fd);
    return 0;
}
