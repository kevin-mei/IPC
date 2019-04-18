#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string>
#include <iostream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

namespace message_queue_systemV {

void msg_stat(int,struct msqid_ds );

void run()
{
    int gflags,sflags,rflags;
    key_t key;
    int msgid;
    int reval;

    struct msgsbuf{
        int mtype;
        char mtext[1];
    }msg_sbuf;
    struct msgmbuf
    {
        int mtype;
        char mtext[10];
    }msg_rbuf;

    struct msqid_ds msg_ginfo,msg_sinfo;
    // char* msgpath="/home/joker/inspur/FY2018/IPC/ShareMemory/msgqueue";
    char* msgpath="/unix/msgqueue";
    key=ftok(msgpath,'a');
    gflags=IPC_CREAT|IPC_EXCL;
    msgid=msgget(key,gflags|00666);
    if(msgid==-1)
    {
        if(17 == errno)
        {
            printf("msg has exist, Just open msg");
            gflags=IPC_EXCL;
            msgid=msgget(key,gflags|00666);
            if(msgid==-1)
            {
                printf("msg create error msg[%s]\n", strerror(errno));
                return;
            }
        }else
        {
            printf("msg create error msg[%s]\n", strerror(errno));
            return;
        }
    }
    //创建一个消息队列后，输出消息队列缺省属性
    msg_stat(msgid,msg_ginfo);
    sflags=IPC_NOWAIT;
    msg_sbuf.mtype=10;
    msg_sbuf.mtext[0]='a';
    reval=msgsnd(msgid,&msg_sbuf,sizeof(msg_sbuf.mtext),sflags);
    if(reval==-1)
    {
        printf("message send error msg[%s]\n", strerror(errno));
    }
    //发送一个消息后，输出消息队列属性
    msg_stat(msgid,msg_ginfo);
    rflags=IPC_NOWAIT|MSG_NOERROR;
    reval=msgrcv(msgid,&msg_rbuf,sizeof(msg_rbuf.mtext), msg_sbuf.mtype, rflags);
    if(reval==-1)
        printf("read msg error msg[%s]\n", strerror(errno));
    else
        printf("read from msg queue %d bytes\n",reval);

    int readnum = 0;
    while(1)
    {
        reval=msgrcv(msgid,&msg_rbuf,4,0,rflags);
        readnum++;
        if (reval == -1)
        {
            if (errno == ENOMSG)
            {
                if(1000 ==readnum)
                {
                    break;
                }
                usleep(100);
                continue;
            }
            else
            {
                printf("msgrcv failed, error = %s\n", strerror(errno));
                break;
            }
        }

        printf("get message: %d bytes mtext[%s]\n",reval, msg_rbuf.mtext);
        break;

        //遇到end结束
//        if(strncmp(data.text, "end", 3) == 0)
//        {
//            break;
//        }

    }
    //从消息队列中读出消息后，输出消息队列属性
    msg_stat(msgid,msg_ginfo);
    msg_sinfo.msg_perm.uid=8;//just a try
    msg_sinfo.msg_perm.gid=8;//
    msg_sinfo.msg_qbytes=16388;
    //此处验证超级用户可以更改消息队列的缺省msg_qbytes
    //注意这里设置的值大于缺省值
    reval=msgctl(msgid,IPC_SET,&msg_sinfo);
    if(reval==-1)
    {
        printf("msg set info error msg[%s]\n", strerror(errno));
        return;
    }
    msg_stat(msgid,msg_ginfo);
    //验证设置消息队列属性
    reval=msgctl(msgid,IPC_RMID,NULL);//删除消息队列
    if(reval==-1)
    {
        printf("unlink msg queue error msg[%s]\n", strerror(errno));
        return;
    }
}

void msg_stat(int msgid,struct msqid_ds msg_info)
{
    int reval;
    sleep(1);//只是为了后面输出时间的方便
    reval=msgctl(msgid,IPC_STAT,&msg_info);
    if(reval==-1)
    {
        printf("get msg info error\n");
        return;
    }
    printf("\n");
    printf("current number of bytes on queue is %d\n",msg_info.msg_cbytes);
    printf("number of messages in queue is %d\n",msg_info.msg_qnum);
    printf("max number of bytes on queue is %d\n",msg_info.msg_qbytes);
    //每个消息队列的容量（字节数）都有限制MSGMNB，值的大小因系统而异。在创建新的消息队列时，//msg_qbytes的缺省值就是MSGMNB
    printf("pid of last msgsnd is %d\n",msg_info.msg_lspid);
    printf("pid of last msgrcv is %d\n",msg_info.msg_lrpid);
    printf("last msgsnd time is %s", ctime(&(msg_info.msg_stime)));
    printf("last msgrcv time is %s", ctime(&(msg_info.msg_rtime)));
    printf("last change time is %s", ctime(&(msg_info.msg_ctime)));
    printf("msg uid is %d\n",msg_info.msg_perm.uid);
    printf("msg gid is %d\n",msg_info.msg_perm.gid);
}

}


namespace message_queue_posix {

#define msgpath "/home/joker/inspur/FY2018/IPC/ShareMemory/msgqueue_posix"

void run()
{
    mqd_t mqID;

    struct mq_attr attr;
    attr.mq_flags = O_RDONLY;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    mode_t omask;
    omask = umask(0);

    mqID = mq_open(msgpath, O_RDWR|O_CREAT|O_EXCL, 0666, &attr);
    umask(omask);

    if (mqID < 0)
    {
        printf("open message queue %s error[%s]\n", msgpath, strerror(errno));
        return ;
    }
    printf("open message queue succ, mqID = %d\n", mqID);

    mq_attr mqAttr;
    if (mq_getattr(mqID, &mqAttr) < 0)
    {
        printf("get the message queue attribute error\n");
        return ;
    }

    printf("mq_flags = %d, mq_maxmsg = %d, mq_msgsize = %d, mq_curmsgs = %d\n",
        mqAttr.mq_flags, mqAttr.mq_maxmsg, mqAttr.mq_msgsize, mqAttr.mq_curmsgs);

    unsigned int n;
    unsigned int iprio = 0;

    std::string strmsg;
    std::cin >> strmsg;

    // msg_prio：消息的优先级；它是一个小于 MQ_PRIO_MAX的数，数值越大，优先级越高。
    // POSIX消息队列在调用mq_receive时总是返回队列中最高优先级的最早消息。如果消息不需
    // 要设定优先级，那么可以在 mq_send是置msg_prio为0，mq_receive的msg_prio置为NULL
    mq_send(mqID, strmsg.c_str(), strmsg.length(), iprio);


    char buff[1024] = {0};

    n = mq_receive(mqID, buff, mqAttr.mq_msgsize, &iprio);
    printf("read from mq`s msg = %s\n", buff);

    return;
}

}
#endif // MESSAGEQUEUE_H
