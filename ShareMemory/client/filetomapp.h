#ifndef FILETOMAPP_H
#define FILETOMAPP_H

namespace filetomapp {

#define sharfile "/home/joker/inspur/FY2018/IPC/ShareMemory/test"

typedef struct{
  char name[4];
  int  age;
}people;

void run()  // map a normal file as shared mem:
{
  int fd,i;
  people *p_map;
  fd=open( sharfile, O_CREAT|O_RDWR,00777 );
  p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ|PROT_WRITE,
       MAP_SHARED,fd,0);
  for(i = 0;i<10;i++)
  {
    printf( "name: %s age %d;\n",(*(p_map+i)).name, (*(p_map+i)).age );
  }
  munmap( p_map,sizeof(people)*10 );

  return ;
}


}


#endif // FILETOMAPP_H
