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

    mc_ctx_t* ctx;

    QVBoxLayout* layout;
    QSplitter* splitter;

    void InitTemplateEditor();
    void InitHexEditor();

public:

    void setMycroftCtx(mc_ctx_t* ctx);
    mc_ctx_t* getMycroftCtx();
    void setMainFile(mc_file_t* file);
    mc_file_t* getMainFile();

    MainEditor(QWidget* parent);
    ~MainEditor();
};

#endif // MAINEDITOR_H
