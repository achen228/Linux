#include <stdio.h>
#include <unistd.h>

int g_val = 100;
int main()
{
      printf("hello world!---pid:%d\n", getpid());
      //pid_t fork(void);
      //创建一个子进程，父进程返回子进程的pid；子进程返回0
      pid_t pid = fork();
      if (pid < 0)
      {
          printf("fork error!\n");
          return -1;
      }
      if (pid == 0)
      {
          //子进程与父进程代码共享、数据独有；即g_val的值是不同的
          //子进程地址与父进程地址相同，但其实是虚拟地址相同
          g_val = 200;
          printf("------child------%d   g_val:%d---%p\n", getpid(), g_val, &g_val);
      }else {
          sleep(2);
          printf("------parent------%d   g_val:%d---%p\n", getpid(), g_val, &g_val);
      }
      printf("nihao~~world!---pid:%d\n", getpid());
      while(1)
      {
          printf("DreamCatching\n");
          sleep(2);
      }
      return 0;
}
