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
    QSplitter* splitter;

    void InitTemplateEditor();
    void InitHexEditor();

public:

    void init();
    MainEditor(QWidget* parent);
    ~MainEditor();

};

#endif // MAINEDITOR_H
