#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QTreeView>

#include "templatemodel.h"

class TemplateEditor : public QTreeView {

    Q_OBJECT

    void init();

public:

    TemplateEditor(QWidget* parent = NULL);

    bool parseTemplate(TemplateModel* model, QString template_text);
    bool parseTemplateFile(TemplateModel* model, QString file);

};

#endif // TEMPLATEEDITOR_H
