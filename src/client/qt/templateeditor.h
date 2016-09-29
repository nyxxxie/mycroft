#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QTreeView>
#include "maineditor.h"
#include "templatemodel.h"

class TemplateEditor : public QTreeView {

    Q_OBJECT

    bool init();

public:

    template_t* getCurTemplate();
    TemplateEditor(MainEditor* parent);
};

#endif // TEMPLATEEDITOR_H
