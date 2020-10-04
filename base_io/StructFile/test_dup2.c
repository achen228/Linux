//这个demo体会关闭1号文件描述符，即标准输出

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    //将文件权限掩码设置为0
    umask(0);

    int fd = open("./test_dup2.txt", O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }

    //将1号文件描述符也指向fd所描述的文件
    dup2(fd, 1);

    printf("fd = %d\n", fd);
    fflush(stdout);

    //关闭文件，释放资源
    close(fd);
    return 0;
}
