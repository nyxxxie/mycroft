#include <mycroft/context.h>
#include "mycroft.h"
#include "ui_mycroft.h"

Mycroft::Mycroft(mc_ctx_t* ctx, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Mycroft)
{
    ui->setupUi(this);
    this->ctx = ctx;
}

Mycroft::~Mycroft()
{
    delete ui;
}
