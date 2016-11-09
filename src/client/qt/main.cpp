#include "mycroft.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mycroft w;
    w.show();

    return a.exec();
}
