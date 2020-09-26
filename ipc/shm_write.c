#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>

#define IPC_KEY 0x12345678

int main()
{
    // 创建共享内存
    int shmid = shmget(IPC_KEY, 32, IPC_CREAT | 0664);
    if(shmid < 0)
    {
        perror("shmget error");
        return -1;
    }

    // 建立映射
    void* shm_start = shmat(shmid, NULL, 0);
    if(shm_start == (void*)-1)
    {
        perror("shmat error");
        return -1;
    }

    // 共享内存的操作
    int i = 0;
    while(1)
    {
        sprintf((char*)shm_start, "今天天气很好--%d", i++);
        sleep(1);
    }

    // 解除映射关系
    shmdt(shm_start);

    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}
