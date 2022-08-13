#include "heartdanceobj.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HeartDanceObj obj;
    obj.showMainWidget();

    return a.exec();
}
