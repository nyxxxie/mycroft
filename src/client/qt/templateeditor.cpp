#include <QFile>
#include <mycroft/mycroft.h>
#include "maineditor.h"
#include "templateeditor.h"
#include "templatemodel.h"

/**
 * Starts up the template viewer with the default struct.
 */
void TemplateEditor::init() {
    this->t = NULL;
    TemplateModel* model = new TemplateModel(this);
    setModel(model);
}

TemplateEditor::TemplateEditor(MainEditor* parent)
    : QTreeView(parent) {
    init();
}

void TemplateEditor::setContext(mc_ctx_t* ctx) {
    mc_file_t* file = mc_project_get_focused_file(mc_ctx_get_focused_project(ctx));
    this->t = mc_file_get_template(file);
}
