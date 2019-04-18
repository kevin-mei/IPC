#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

namespace pipe_write_Broken {

/* 写规则的验证1：***写端对读端存在的依赖性***
 * 只有在管道的读端存在时，向管道中写入数据才有意义。否则，向管道中写入数据
 * 的进程将收到内核传来的SIFPIPE信号，应用程序可以处理该信号，也可以忽略（默认动作则是应用程序终止）
 * 输出结果为： Broken pipe*/

void run()
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    char* w_buf;
    int writenum;
    int cmd;

    memset(r_buf,0,sizeof(r_buf));
    if(pipe(pipe_fd)<0)
    {
        printf("pipe create error\n");
        return ;
    }

    if((pid=fork())==0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        sleep(10);
        exit(0);
    }
    else if(pid>0)
    {
        sleep(1);  //等待子进程完成关闭读端的操作
        close(pipe_fd[0]);//write
        w_buf="111";
        if((writenum=write(pipe_fd[1],w_buf,4))==-1)
            printf("write to pipe error\n");
        else
            printf("the bytes write to pipe is %d \n", writenum);

        close(pipe_fd[1]);
    }
}

}

namespace pipe_write_NotAtom {

/*对管道的写规则的验证2：linux不保证写管道的原子性验证
 *输出结果：
the bytes write to pipe 1000
the bytes write to pipe 1000  //注意，此行输出说明了写入的非原子性
the bytes write to pipe 1000
the bytes write to pipe 1000
the bytes write to pipe 1000
the bytes write to pipe 120  //注意，此行输出说明了写入的非原子性
the bytes write to pipe 0
the bytes write to pipe 0
 **/
void run()
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4096];
    char w_buf[4096*2];
    int writenum;
    int rnum;
    memset(r_buf,0,sizeof(r_buf));
    if(pipe(pipe_fd)<0)
    {
        printf("pipe create error\n");
        return ;
    }

    if((pid=fork())==0)
    {
        close(pipe_fd[1]);
        while(1)
        {
            sleep(1);
            rnum=read(pipe_fd[0],r_buf,1000);
            printf("child: readnum is %d\n",rnum);
        }
        close(pipe_fd[0]);

        exit(0);
    }
    else if(pid>0)
    {
        close(pipe_fd[0]);//write
        memset(r_buf,0,sizeof(r_buf));
        if((writenum=write(pipe_fd[1],w_buf,1024))==-1)
            printf("write to pipe error\n");
        else
            printf("the bytes write to pipe is %d \n", writenum);
        writenum=write(pipe_fd[1],w_buf,4096);
        close(pipe_fd[1]);
    }
}

}


namespace pipe_application {

/*
 *输出结果：
child: the cmd from parent is 3
child: the cmd from parent is 5
child: invalid command
brokeny
 **/

int handle_cmd(int cmd);

void run()
{
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    char** w_buf[256];
    int childexit=0;
    int i;
    int cmd;

    memset(r_buf,0,sizeof(r_buf));
    if(pipe(pipe_fd)<0)
    {
        printf("pipe create error\n");
        return ;
    }
    if((pid=fork())==0)
    //子进程：解析从管道中获取的命令，并作相应的处理
    {
        printf("\n");
        close(pipe_fd[1]);
        sleep(2);

        while(!childexit)
        {
            read(pipe_fd[0],r_buf,4);
            cmd=atoi(r_buf);
            if(cmd==0)
            {
                printf("child: receive command from parent over\n now child process exit\n");
                childexit=1;
            }
            else if(handle_cmd(cmd)!=0)
                return;
            sleep(1);
        }
        close(pipe_fd[0]);
        exit(0);
    }
    else if(pid>0)
    //parent: send commands to child
    {
        close(pipe_fd[0]);
        w_buf[0]=(char**)"003";
        w_buf[1]=(char**)"005";
        w_buf[2]=(char**)"777";
        w_buf[3]=(char**)"000";
        for(i=0;i<4;i++)
            write(pipe_fd[1],w_buf[i],4);
        close(pipe_fd[1]);
    }
}
//下面是子进程的命令处理函数（特定于应用）：
int handle_cmd(int cmd)
{
    if((cmd<0)||(cmd>256))
    //suppose child only support 256 commands
    {
        printf("child: invalid command \n");
        return -1;
    }
    printf("child: the cmd from parent is %d\n", cmd);
    return 0;
}

}

#endif // PIPE_H
