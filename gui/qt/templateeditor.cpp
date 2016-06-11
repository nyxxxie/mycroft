#include <QFile>

#include "templateeditor.h"

//TODO, eliminate these both
#include "treemodel.h"
#include "treeitem.h"


/**
 * Starts up the template viewer with the default struct.
 */
void TemplateEditor::init() {
    //TemplateModel model;
    //parseTemplate(&model, "struct FILE {};");

    QStringList headers;
    headers << tr("Title") << tr("Description");

    QFile file("default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel* model = new TreeModel(headers, file.readAll());
    file.close();

    setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        resizeColumnToContents(column);
}

TemplateEditor::TemplateEditor()
    : QTreeView() {
    init();
}

/**
 * Forms a treemodel from a template.
 *
 * @param template_text Text of the template we want to pase.
 * @return Returns true on success, false otherwise.
 */
bool TemplateEditor::parseTemplate(TemplateModel* model, QString template_text) {
    //model->initFromTemplate(template_text);
}

/**
 * Forms a treemodel from a template file.
 *
 * @param template_text Text of the template we want to pase.
 * @return Returns true on success, false otherwise.
 */

bool TemplateEditor::parseTemplateFile(TemplateModel* model, QString file) {

    //TODO: Read template from file

    parseTemplate(model, "");
}
