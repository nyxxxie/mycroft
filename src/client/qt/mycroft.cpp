#include "mycroft.h"
#include "ui_mycroft.h"

Mycroft::Mycroft(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Mycroft) {
    ui->setupUi(this);
}

Mycroft::~Mycroft() {
    delete ui;
}
