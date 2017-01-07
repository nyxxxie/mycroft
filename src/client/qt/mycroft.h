#ifndef MYCROFT_QT_H
#define MYCROFT_QT_H

#include <QMainWindow>
#include <mycroft/project.h>
#include <mycroft/file.h>

class MainHexEditor;

namespace Ui {
class Mycroft;
}

class Mycroft : public QMainWindow
{
    Q_OBJECT

    Ui::Mycroft* ui;

    MainHexEditor* editor;

public:
    explicit Mycroft(QWidget* parent = 0);
    ~Mycroft();

private slots:
    void on_action_file_open();
    void on_action_file_exit();
    void on_action_undo();
    void on_action_redo();
    void on_action_cut();
    void on_action_copy();
    void on_action_paste();
    void on_action_delete();
    void on_action_selectall();
    void on_action_script_run_file();
    void on_action_project_new();
    void on_action_project_open();
    void on_action_template_open();

public slots:
    bool addFile(mc_file_t* file);
    bool addFile(mc_project_t* proj, mc_file_t* file);
    bool removeFile(mc_file_t* file);
    bool removeFile(mc_project_t* proj, mc_file_t* file);
    bool setFocusedFile(mc_project_t* proj, mc_file_t* file);
    bool addProject(mc_project_t* proj);
    bool removeProject(mc_project_t* proj);
    bool setFocusedProject(mc_project_t* proj);

signals:
    void contextChanged();
    void fileAdded(mc_project_t* proj, mc_file_t* file);
    void fileRemoved(mc_project_t* proj, mc_file_t* file);
    void fileFocused(mc_file_t* file);
    void projectAdded(mc_project_t* proj);
    void projectRemoved(mc_project_t* proj);
    void projectFocused(mc_project_t* proj);
};

#endif // MYCROFT_QT_H
