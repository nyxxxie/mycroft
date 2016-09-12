#include <QFile>

#include "templateeditor.h"
#include "templatemodel.h"

/**
 * Starts up the template viewer with the default struct.
 */
void TemplateEditor::init() {
    //TemplateModel model;
    //parseTemplate(&model, "struct FILE {};");

    TemplateModel* model = new TemplateModel();

    setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        resizeColumnToContents(column);
}

TemplateEditor::TemplateEditor(QWidget* parent)
    : QTreeView(parent) {
    init();
}

//ast_struct_t* getAstEntry() {
//    return ((MainEditor*)parent())->getMycroftCtx()->templ->entry;
//}
