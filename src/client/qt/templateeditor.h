#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QTreeView>
#include <mycroft/mycroft.h>
#include "templatemodel.h"

class TemplateEditor : public QTreeView {
    friend class TemplateModel;

    Q_OBJECT

    void init();

protected:
    template_t* t;

public:
    TemplateEditor(QWidget* parent);

public slots:
    void setFile(mc_file_t* file);
};

#endif // TEMPLATEEDITOR_H
