#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "templateeditor.h"

class MainEditor : public QWidget {

    Q_OBJECT

    QTextEdit* hexedit;
    TemplateEditor* tedit;

    QVBoxLayout* layout;
    QSplitter* splitter;

    void InitTemplateEditor();
    void InitHexEditor();

public:

    MainEditor();
    ~MainEditor();
};

#endif // MAINEDITOR_H
