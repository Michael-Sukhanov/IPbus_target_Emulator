#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setFont(QFont("Consolas", 8));
    QCoreApplication::setApplicationName("IPbus Emulator");
    QCoreApplication::setApplicationVersion("3.0");
    MainWindow w;
    w.show();
    return a.exec();
}
