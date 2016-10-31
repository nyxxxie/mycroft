#include <QTreeView>
#include "hexeditor.h"
#include "templateeditor.h"
#include "maineditor.h"

void MainEditor::InitTemplateEditor() {
    tedit = new TemplateEditor(this);
}

void MainEditor::InitHexEditor() {
    hexedit = new HexEditor(this);
}

void MainEditor::CreateEditorSplit() {
    lsp1 = new QSplitter;
    lsp1->setOrientation(Qt::Horizontal);
    lsp1->addWidget(new QTextEdit);
    lsp1->addWidget(hexedit);
    lsp1->setStretchFactor(0, 0);
    lsp1->setStretchFactor(1, 1);

    c1_l = new QVBoxLayout;
    c1_l->addWidget(lsp1);
    c1_l->setContentsMargins(0,0,0,0); // removes the weird border

    c1 = new QWidget;
    c1->setLayout(c1_l);
}

void MainEditor::CreateTemplateSplit() {
    lsp2 = new QSplitter;
    lsp2->setOrientation(Qt::Horizontal);
    lsp2->addWidget(new QTextEdit);
    lsp2->addWidget(tedit);
    lsp2->setStretchFactor(0, 0);
    lsp2->setStretchFactor(1, 1);

    c2_l = new QVBoxLayout;
    c2_l->addWidget(lsp2);
    c2_l->setContentsMargins(0,0,0,0); // removes the weird border

    c2 = new QWidget;
    c2->setLayout(c2_l);
}

void MainEditor::init() {

    /* Create the two main elements of this widget */
    InitTemplateEditor();
    InitHexEditor();

    CreateEditorSplit();
    CreateTemplateSplit();

    vertsplit = new QSplitter;
    vertsplit->setOrientation(Qt::Vertical);
    vertsplit->addWidget(c1);
    vertsplit->addWidget(c2);
    vertsplit->setStretchFactor(0, 1);
    vertsplit->setStretchFactor(1, 0);

    /* Place split in layout */
    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0); // removes the weird border
    layout->addWidget(vertsplit);

    setLayout(layout);

    connect(parent(), SIGNAL(fileFocused(mc_file_t*)),
            hexedit, SLOT(setFile(mc_file_t*)));
    connect(parent(), SIGNAL(fileFocused(mc_file_t*)),
            tedit, SLOT(setFile(mc_file_t*)));
}

MainEditor::MainEditor(QWidget* parent)
    : QWidget(parent) {
}

MainEditor::~MainEditor() {
    delete hexedit;
    delete tedit;
    delete layout;
    delete vertsplit;
    delete c1;
    delete c2;
    delete lsp1;
    delete lsp2;
    delete c1_l;
    delete c2_l;
}
