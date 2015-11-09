#include "ggmainwindow.h"
#include <QApplication>

#ifdef USE_VLD
#include <vld.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GGMainWindow w;
    w.show();

    return a.exec();
}
