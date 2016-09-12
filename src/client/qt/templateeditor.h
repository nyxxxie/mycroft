#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QTreeView>

#include "templatemodel.h"

class TemplateEditor : public QTreeView {

    Q_OBJECT

    void init();

public:

    TemplateEditor(QWidget* parent);
    ast_struct_t* getAstEntry();
};

#endif // TEMPLATEEDITOR_H
