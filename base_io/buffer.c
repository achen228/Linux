#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    printf("%s", "printf");
    fprintf(stdout, "%s", "fprintf");
    fwrite("fwrite", 6, 1, stdout);
    write(1, "write", 5);
    sleep(2);
    return 0;
}
