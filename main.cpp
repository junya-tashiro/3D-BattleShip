#include "start.h"

#include <QApplication>
#include <QMenu>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    start w;
    w.show();

    return a.exec();
}
