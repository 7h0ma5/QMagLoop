#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("DL2IC");
    QCoreApplication::setOrganizationDomain("dl2ic.de");
    QCoreApplication::setApplicationName("QMagLoop");

    MainWindow w;
    w.show();
    return a.exec();
}
