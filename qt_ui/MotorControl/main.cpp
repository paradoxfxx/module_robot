#include "motorcontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotorControl w;
    w.show();

    return a.exec();
}
