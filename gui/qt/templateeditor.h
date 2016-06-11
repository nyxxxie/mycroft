#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QTreeWidget>

class TemplateEditor : public QTreeWidget {

    Q_OBJECT

private slots:

    void ShowDirectory(QTreeWidgetItem* item, int column);

public:

    TemplateEditor();

    void Init();
};

#endif // TEMPLATEEDITOR_H
