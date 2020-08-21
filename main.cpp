#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setFont(QFont("consolas", 8));
    MainWindow w;
    w.show();
    return a.exec();
}
