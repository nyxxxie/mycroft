#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    //TODO: Set up argument parser

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
