#include <mycroft/context.h>
#include "mainhexeditor.h"
#include "ui_mainhexeditor.h"

MainHexEditor::MainHexEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainHexEditor)
{
    ui->setupUi(this);

    /* */
    connect(parent, SIGNAL(fileFocused(mc_file_t*)),
            ui->hexeditor, SLOT(setFile(mc_file_t*)));
    connect(parent, SIGNAL(fileFocused(mc_file_t*)),
            ui->templateeditor, SLOT(setFile(mc_file_t*)));
    connect(parent, SIGNAL(contextChanged(mc_ctx_t*)),
            ui->projectview, SLOT(setContext(mc_ctx_t*)));
    connect(ui->projectview, SIGNAL(focusProject(mc_ctx_t*, mc_project_t*)),
            parent, SLOT(setFocusedProject(mc_ctx_t*, mc_project_t*)));
    connect(ui->projectview, SIGNAL(focusFile(mc_project_t*, mc_file_t*)),
            parent, SLOT(setFocusedFile(mc_project_t*, mc_file_t*)));

    // TODO: manually create hexxeitor and templateeditor here and add them to ui splitters?

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
