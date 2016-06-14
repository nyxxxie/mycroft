#include "maineditor.h"

void MainEditor::InitTemplateEditor() {
    tedit = new TemplateEditor();
}

void MainEditor::InitHexEditor() {
    hexedit = new QTextEdit();
}

MainEditor::MainEditor() : QWidget() {

    /* Create the two main elements of this widget */
    InitTemplateEditor();
    InitHexEditor();

    /* Place each element in the widget split */
    QSplitter* splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(hexedit);
    splitter->addWidget(tedit);

    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(splitter);

    setLayout(layout);
}

MainEditor::~MainEditor() {
    delete hexedit;
    delete tedit;
    delete layout;
    delete splitter;
}
