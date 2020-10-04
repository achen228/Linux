#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main()
{   
    umask(0);
    int fd = open("./myfile.txt", O_RDWR | O_CREAT, 0777);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }

    //每次将写位置跳转到文件末尾，相当于追加写
    lseek(fd, 0, SEEK_END);

    const char* str = "hello world\n";
    ssize_t ret = write(fd, str, strlen(str));
    if(ret < 0)
    {
        perror("write error");
        return -1;
    }
    printf("ret:%d\n", (int)ret);

    //跳转到文件的起始位置，否则会读取不到数据，因为此时文件指针在末尾
    lseek(fd, 0, SEEK_SET);

    char buf[1024] = {0};
    ret = read(fd, buf, sizeof(buf) - 1);
    if(ret < 0)
    {
        perror("read error");
        return -1;
    }
    printf("ret:%d---buf:%s\n", (int)ret, buf);

    //关闭文件
    close(fd);
    return 0;
}
