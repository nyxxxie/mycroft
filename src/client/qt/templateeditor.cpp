#include <QFile>

#include "templateeditor.h"
#include "templatemodel.h"

/**
 * Starts up the template viewer with the default struct.
 */
void TemplateEditor::init() {
    //TemplateModel model;
    //parseTemplate(&model, "struct FILE {};");

    QFile file(":/resources/default.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    TemplateModel* model = new TemplateModel(file.readAll());
    setModel(model);
    file.close();
    for (int column = 0; column < model->columnCount(); ++column)
        resizeColumnToContents(column);
}

TemplateEditor::TemplateEditor(QWidget* parent)
    : QTreeView(parent) {
    init();
}
