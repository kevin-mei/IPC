#include <QCoreApplication>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "pagesize.h"
#include "filetomapp.h"
#include "systemv_read.h"
#include "pipe_read.h"


// Todo: modify namespace to test different sharemem ways
// using namespace filetomapp;
// using namespace anonymousmapping;
// using namespace pagesize;
using namespace systemV_read;
using namespace pipe_read;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    run();

    return a.exec();
}
