#include <QCoreApplication>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "pagesize.h"
#include "anonymousmapping.h"
#include "filetomapp.h"

// Todo: modify namespace to test different sharemem ways
//using namespace filetomapp;
//using namespace anonymousmapping;
using namespace pagesize;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    run();

    return a.exec();
}
