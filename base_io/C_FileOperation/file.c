#include <stdio.h>
#include <string.h>

int main()
{
    FILE* fp = fopen("./myfile.txt", "r+");
    if(!fp)
    {
        printf("fopen error\n");
        return -1;
    }

    const char* str = "hello world\n";
    size_t wnum = fwrite(str, 1, strlen(str), fp);
    if(wnum == 0)
    {
        printf("fwrite error\n");
        return -1;
    }
    printf("wnum:%d\n", (int)wnum);

    //跳转到文件的起始位置，否则会读取不到数据，因为此时的文件指针在末尾
    fseek(fp, 0, SEEK_SET);

    char buf[1024] = {0};
    size_t rnum = fread(buf, 1, sizeof(buf) - 1, fp);
    printf("rnum:%d --- buf:%s\n", (int)rnum, buf);
    return 0;
}
