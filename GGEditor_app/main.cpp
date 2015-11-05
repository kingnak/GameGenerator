#include "ggmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GGMainWindow w;
    w.show();

    return a.exec();
}
