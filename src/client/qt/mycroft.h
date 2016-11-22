#ifndef MYCROFT_H
#define MYCROFT_H

#include <QMainWindow>
#include <mycroft/context.h>

class MainHexEditor;

namespace Ui {
class Mycroft;
}

class Mycroft : public QMainWindow
{
    Q_OBJECT

    Ui::Mycroft* ui;
    mc_ctx_t* ctx;

    MainHexEditor* editor;

public:
    explicit Mycroft(mc_ctx_t* ctx, QWidget* parent = 0);
    ~Mycroft();

public slots:
    void setContext(mc_ctx_t* ctx);
    bool addFile(mc_file_t* file);
    bool addFile(mc_project_t* proj, mc_file_t* file);
    bool removeFile(mc_file_t* file);
    bool removeFile(mc_project_t* proj, mc_file_t* file);
    bool addProject(mc_project_t* proj);
    bool removeProject(mc_project_t* proj);

signals:
    void contextChanged(mc_ctx_t* ctx);
    void fileAdded(mc_project_t* proj, mc_file_t* file);
    void fileRemoved(mc_project_t* proj, mc_file_t* file);
    void fileFocused(mc_file_t* file);
    void projectAdded(mc_project_t* proj);
    void projectRemoved(mc_project_t* proj);
    void projectFocused(mc_project_t* proj);
};

#endif // MYCROFT_H
