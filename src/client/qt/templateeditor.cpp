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
    this->t = file_get_template(mycroft_get_file(ctx));
}
