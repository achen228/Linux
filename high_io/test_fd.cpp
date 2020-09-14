#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        return 0;
    }

    int flag = fcntl(fd[0], F_GETFL);
    printf("read flag = %d\n", flag);
    flag = fcntl(fd[1], F_GETFL);
    printf("write flag = %d\n", flag);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    flag = fcntl(sockfd, F_GETFL);
    printf("sockfd flag = %d\n", flag);
    
    fcntl(sockfd, F_SETFL, flag | O_ASYNC);
    flag = fcntl(sockfd, F_GETFL);
    printf("sockfd flag = %d\n", flag);

    return 0;
}
