#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd = open("./test.txt", O_CREAT | O_RDWR, 0664);
    if(fd < 0)
    {
        perror("open error");
        return -1;

    }
    printf("fd:%d\n", fd);
    close(fd);
    return 0;
}
