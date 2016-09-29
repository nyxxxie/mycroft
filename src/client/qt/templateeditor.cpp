#include <QFile>
#include <mycroft/mycroft.h>
#include "maineditor.h"
#include "templateeditor.h"
#include "templatemodel.h"

/**
 * Starts up the template viewer with the default struct.
 */
bool TemplateEditor::init() {
    TemplateModel* model = new TemplateModel(this);
    setModel(model);
}

template_t* TemplateEditor::getCurTemplate() {
    mc_file_t* file = ((MainEditor*)parent())->getMainFile();
    return file_get_template(file);
}

TemplateEditor::TemplateEditor(MainEditor* parent)
    : QTreeView(parent) {
    init();
}
