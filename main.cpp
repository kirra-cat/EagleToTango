#include "eagletotango.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EagleToTango w;
    w.show();

    return a.exec();
}
