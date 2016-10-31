#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <mycroft/mycroft.h>

class HexEditor;
class TemplateEditor;

class MainEditor : public QWidget {

    Q_OBJECT

    HexEditor* hexedit;
    TemplateEditor* tedit;

    QVBoxLayout* layout;
    QSplitter* vertsplit;

    void InitTemplateEditor();
    void InitHexEditor();

    QWidget* c1;
    QVBoxLayout* c1_l;
    QSplitter* lsp1;
    void CreateEditorSplit();

    QWidget* c2;
    QVBoxLayout* c2_l;
    QSplitter* lsp2;
    void CreateTemplateSplit();

public:

    void init();
    MainEditor(QWidget* parent);
    ~MainEditor();

};

#endif // MAINEDITOR_H
