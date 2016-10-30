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

void TemplateEditor::setFile(mc_file_t* file) {
    if (file == NULL) {
        this->t = NULL;
    }
    else {
        this->t = mc_file_get_template(file);
    }
}
