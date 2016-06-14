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
