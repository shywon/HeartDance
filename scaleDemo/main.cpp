#include "zsharescreenobj.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ZShareScreenObj obj;
    obj.showTool();

    return a.exec();
}
