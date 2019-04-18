#include <QCoreApplication>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "pagesize.h"
#include "anonymousmapping.h"
#include "filetomapp.h"
#include "systemv_write.h"
#include "pipe_write.h"
#include "messagequeue.h"

// Todo: modify namespace to test different sharemem ways

// using namespace filetomapp;
// using namespace anonymousmapping;
// using namespace pagesize;
// using namespace systemV_write;
// using namespace pipe_write_Broken;
//using namespace pipe_write_NotAtom;
//using namespace pipe_application;
using namespace message_queue_systemV;

//using namespace message_queue_posix;
//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    run();

//    return a.exec();
//}

int main()
{

    run();

    return 0;
}
