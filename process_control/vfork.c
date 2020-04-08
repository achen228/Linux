#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    //pid_t vfork(void);
    int pid = vfork();
    if (pid == 0)
    {
        printf("---child---\n");
        sleep(5);
        exit(0);
    }
    printf("---parent---\n");
    return 0;
}
