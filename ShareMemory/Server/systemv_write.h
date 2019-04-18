#ifndef SYSTEMV_WRITE_H
#define SYSTEMV_WRITE_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
namespace systemV_write {

typedef struct{
    char name[4];
    int age;
} people;

void run()
{
    int shm_id,i;
    key_t key;
    char temp;
    people *p_map;
    char* name = "/dev/shm/myshm2";
    key = ftok(name,0);
    if(key==-1)
    {
        perror("ftok error");
        //perror("ftok error,please confirm %s is exist and can access, stderr[%s]" ,name, stderr);
        return ;
    }

    shm_id = shmget(key, 0, 0);

    //如果共享内存区已经存在，则清空该共享内存区域。
    if (shm_id != -1)
    {
        shmctl(shm_id, IPC_RMID, 0);
    }

    shm_id=shmget(key,4096, 0666|IPC_CREAT);
    if(shm_id==-1)
    {
        perror("shmget error");
        return;
    }
    p_map=(people*)shmat(shm_id,NULL,0);
    if (p_map == (void *)-1)
    {
        perror("shmat failed");
        return;
    }
    temp='a';
    for(i = 0;i<10;i++)
    {
        temp+=1;
        memcpy((*(p_map+i)).name,&temp,1);
        (*(p_map+i)).age=20+i;
    }
    if(shmdt(p_map)==-1)
        perror(" detach error ");
    return;
}

}
#endif // SYSTEMV_WRITE_H
