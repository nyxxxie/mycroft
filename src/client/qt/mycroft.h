#ifndef MYCROFT_H
#define MYCROFT_H

#include <QMainWindow>
#include <mycroft/context.h>

namespace Ui {
class Mycroft;
}

class Mycroft : public QMainWindow
{
    Q_OBJECT

    mc_ctx_t* ctx;

public:
    explicit Mycroft(mc_ctx_t* ctx, QWidget *parent = 0);
    ~Mycroft();

private:
    Ui::Mycroft *ui;
};

#endif // MYCROFT_H
