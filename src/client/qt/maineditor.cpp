#include "hexeditor.h"
#include "templateeditor.h"
#include "maineditor.h"

void MainEditor::InitTemplateEditor() {
    tedit = new TemplateEditor(this);
}

void MainEditor::InitHexEditor() {
    hexedit = new HexEditor(this);
}

void MainEditor::setMycroftCtx(mc_ctx_t* ctx) {
    this->ctx = ctx;
}

mc_ctx_t* MainEditor::getMycroftCtx() {
    return this->ctx;
}

void MainEditor::setMainFile(mc_file_t* file) {

}

mc_file_t* MainEditor::getMainFile() {
    return mycroft_get_file(ctx);
}

void MainEditor::init() {

    /* Create the two main elements of this widget */
    InitTemplateEditor();
    InitHexEditor();

    /* Place each element in the widget split */
    QSplitter* splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(hexedit);
    splitter->addWidget(tedit);

    /* Make the bottom editor maintain split size on window resize */
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    /* Place split in layout */
    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0); // removes the weird border
    layout->addWidget(splitter);

    setLayout(layout);
}

MainEditor::MainEditor(QWidget* parent)
    : QWidget(parent) {
}

MainEditor::~MainEditor() {
    delete hexedit;
    delete tedit;
    delete layout;
    delete splitter;
}
