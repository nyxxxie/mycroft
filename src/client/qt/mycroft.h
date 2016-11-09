#ifndef MYCROFT_H
#define MYCROFT_H

#include <QMainWindow>

namespace Ui {
class Mycroft;
}

class Mycroft : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mycroft(QWidget *parent = 0);
    ~Mycroft();

private:
    Ui::Mycroft *ui;
};

#endif // MYCROFT_H
