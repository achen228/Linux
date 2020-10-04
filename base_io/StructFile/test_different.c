//这个demo体会库函数接口和系统调用接口 缓冲区的有无及输出

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    //库函数接口必须刷新缓冲区后才会将数据打印，程序退出前会刷新缓冲区，所以最后打印
    fprintf(stdout, "%s---%d", "fprintf", 1);
    fwrite("fwrite", 1, 6, stdout);

    printf("printf");

    //系统调用接口直接将数据写入到文件中，所以先打印
    write(1, "write", 5);

    sleep(3);
    return 0;
}
