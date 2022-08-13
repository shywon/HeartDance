#include "sharescreenobj.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ShareScreenObj obj;
    obj.showTool();

    return a.exec();
}
