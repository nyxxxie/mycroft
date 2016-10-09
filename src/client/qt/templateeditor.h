#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QString>
#include <QTreeView>
#include <mycroft/mycroft.h>
#include "maineditor.h"
#include "templatemodel.h"

class TemplateEditor : public QTreeView {
    friend class TemplateModel;

    Q_OBJECT

    void init();

protected:
    template_t* t;

public:
    TemplateEditor(MainEditor* parent);

public slots:
    void setContext(mc_ctx_t* ctx);
};

#endif // TEMPLATEEDITOR_H
