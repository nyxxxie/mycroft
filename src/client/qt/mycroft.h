/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#ifndef MYCROFT_QT_H
#define MYCROFT_QT_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
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
    bool createPythonTerminal();
    bool createWindowToggleMenu();
    bool destroyMainEditor();
    bool destroyTemplateEditor();
    bool destroyProjectView();
    bool destroyPythonTerminal();
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
    QMenu* menu_dockable_widgets;

    ProjectView* projectview;
    QDockWidget* dock_projectview;
    TemplateEditor* templateeditor;
    QDockWidget* dock_templateeditor;
    QTextEdit* pyterm;
    QDockWidget* dock_pyterm;

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
    void on_action_project_saveas();
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
