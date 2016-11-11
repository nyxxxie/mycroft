#include "mainhexeditor.h"
#include "ui_mainhexeditor.h"

MainHexEditor::MainHexEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainHexEditor)
{
    ui->setupUi(this);

    /* Make splitters behave properly */
    ui->central_split->setStretchFactor(0, 0);
    ui->central_split->setStretchFactor(1, 1);
    ui->editor_separator->setStretchFactor(1, 0);
    ui->editor_separator->setStretchFactor(0, 1);
    ui->tool_separator->setStretchFactor(1, 0);
    ui->tool_separator->setStretchFactor(0, 1);
}

MainHexEditor::~MainHexEditor()
{
    delete ui;
}
