#ifndef SYSTEMV_READ_H
#define SYSTEMV_READ_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
namespace systemV_read {

typedef struct{
    char name[4];
    int age;
} people;
void run()
{
    int shm_id,i;
    key_t key;
    people *p_map;
    char* name = "/dev/shm/myshm2";
    key = ftok(name,0);
    if(key == -1)
    {
        perror("ftok error");
        return;
    }
//    shm_id = shmget(key, 0, 0);
//    if (shm_id != -1)
//    {
//        shmctl(shm_id, IPC_RMID, 0);
//    }

    shm_id = shmget(key,4096,0666|IPC_CREAT);
    if(shm_id == -1)
    {
        perror("shmget error");
        return;
    }
    p_map = (people*)shmat(shm_id,NULL,0);
    for(i = 0;i<10;i++)
    {
    printf( "name:%s\n",(*(p_map+i)).name );
    printf( "age %d\n",(*(p_map+i)).age );
    }
    if(shmdt(p_map) == -1)
        perror(" detach error ");
    return;
}

}
#endif // SYSTEMV_READ_H
