//这个demo体会关闭1号文件描述符，即标准输出

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    //将文件权限掩码设置为0
    umask(0);

    //关闭标准输出文件描述符
    close(1);

    int fd = open("./test.txt", O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }

    printf("fd = %d\n", fd);
    fflush(stdout);

    //关闭文件，释放资源
    close(fd);
    return 0;
}
