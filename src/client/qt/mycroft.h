#ifndef MYCROFT_QT_H
#define MYCROFT_QT_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <mycroft/project.h>
#include <mycroft/file.h>
#include "maineditor.h"
#include "templateeditor.h"
#include "hexeditor.h"
#include "projectview.h"

class MainHexEditor;

namespace Ui {
class Mycroft;
}

class Mycroft : public QMainWindow
{
    Q_OBJECT

    Ui::Mycroft* ui;

private:
    bool connectMenuActions();
    bool createMainEditor();
    bool createTemplateEditor();
    bool createProjectView();
    bool createWindowToggleMenu();
    bool destroyMainEditor();
    bool destroyTemplateEditor();
    bool destroyProjectView();
    bool destroyWindowToggleMenu();

    bool openFile(QString filename);
    bool openProject(QString projectname);
    bool openTemplate(QString templatename);

public:
    explicit Mycroft(QWidget* parent = 0);
    ~Mycroft();

private:
    MainEditor* editor;
    HexEditor* hexeditor; // TODO: delete me and place this in maineditor
    ProjectView* projectview;
    QDockWidget* dock_projectview;
    TemplateEditor* templateeditor;
    QDockWidget* dock_templateeditor;
    QMenu* menu_dockable_widgets;

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
