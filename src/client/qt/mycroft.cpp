#include <QFileDialog>
#include <QDockWidget>
#include <QMessageBox>
#include <mycroft/mycroft.h>
#include <mycroft/file.h>
#include <mycroft/script.h>
#include <mycroft/error.h>
#include "mycroft.h"
#include "ui_mycroft.h"

Mycroft::Mycroft(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Mycroft)
{
    editor = NULL;
    hexeditor = NULL;
    menu_dockable_widgets = NULL;
    projectview = NULL;
    dock_projectview = NULL;
    templateeditor = NULL;
    dock_templateeditor = NULL;
    pyterm = NULL;
    dock_pyterm = NULL;

    ui->setupUi(this);

    createMainEditor();
    createProjectView();
    createPythonTerminal();
    createTemplateEditor();
    connectMenuActions();
    createWindowToggleMenu();

    tabifyDockWidget(dock_pyterm, dock_templateeditor);
}

Mycroft::~Mycroft()
{
    delete ui;
    delete editor;
}

bool Mycroft::connectMenuActions()
{
    connect(ui->action_file_open, SIGNAL(triggered()), this, SLOT(on_action_file_open()));
    connect(ui->action_file_exit, SIGNAL(triggered()), this, SLOT(on_action_file_exit()));
    connect(ui->action_undo, SIGNAL(triggered()), this, SLOT(on_action_undo()));
    connect(ui->action_redo, SIGNAL(triggered()), this, SLOT(on_action_redo()));
    connect(ui->action_cut, SIGNAL(triggered()), this, SLOT(on_action_cut()));
    connect(ui->action_copy, SIGNAL(triggered()), this, SLOT(on_action_copy()));
    connect(ui->action_paste, SIGNAL(triggered()), this, SLOT(on_action_paste()));
    connect(ui->action_delete, SIGNAL(triggered()), this, SLOT(on_action_delete()));
    connect(ui->action_selectall, SIGNAL(triggered()), this, SLOT(on_action_selectall()));
    connect(ui->action_script_run_file, SIGNAL(triggered()), this, SLOT(on_action_script_run_file()));
    connect(ui->action_project_new, SIGNAL(triggered()), this, SLOT(on_action_project_new()));
    connect(ui->action_project_open, SIGNAL(triggered()), this, SLOT(on_action_project_open()));
    connect(ui->action_project_saveas, SIGNAL(triggered()), this, SLOT(on_action_project_saveas()));
    connect(ui->action_template_open, SIGNAL(triggered()), this, SLOT(on_action_template_open()));
}

bool Mycroft::createMainEditor()
{
    //editor = new MainEditor(this);
    //ui->windowlayout->addWidget(editor);
    hexeditor = new HexEditor(this);
    connect(this, SIGNAL(fileFocused(mc_file_t*)),
            hexeditor, SLOT(setFile(mc_file_t*)));
    ui->windowlayout->addWidget(hexeditor);
}

bool Mycroft::createTemplateEditor()
{
    /* Create dock */
    dock_templateeditor = new QDockWidget("Template", this);
    dock_templateeditor->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea);

    /* Create project view and add to dock */
    templateeditor = new TemplateEditor(this);
    dock_templateeditor->setWidget(templateeditor);

    /* Connect templateeditor and the main window */
    connect(this, SIGNAL(fileFocused(mc_file_t*)),
            templateeditor, SLOT(setFile(mc_file_t*)));

    /* Add dock to window */
    addDockWidget(Qt::BottomDockWidgetArea, dock_templateeditor);

}

bool Mycroft::createProjectView()
{
    /* Create dock */
    dock_projectview = new QDockWidget("Projects", this);
    dock_projectview->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    /* Connect projectview and the main window */
    projectview = new ProjectView(this);
    dock_projectview->setWidget(projectview);

    /* Connect projectview signals with our slots */
    connect(projectview, SIGNAL(focusProject(mc_project_t*)),
            this, SLOT(setFocusedProject(mc_project_t*)));
    connect(projectview, SIGNAL(focusFile(mc_project_t*, mc_file_t*)),
            this, SLOT(setFocusedFile(mc_project_t*, mc_file_t*)));
    connect(this, SIGNAL(fileAdded(mc_project_t*, mc_file_t*)),
            projectview, SLOT(fileAdded(mc_project_t*, mc_file_t*)));
    connect(this, SIGNAL(projectAdded(mc_project_t*)),
            projectview, SLOT(projectAdded(mc_project_t*)));

    /* Add dock to window */
    addDockWidget(Qt::LeftDockWidgetArea, dock_projectview);
}

bool Mycroft::createPythonTerminal()
{
    /* Create dock */
    dock_pyterm = new QDockWidget("Python Terminal", this);
    dock_pyterm->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea);

    /* Create project view and add to dock */
    pyterm = new QTextEdit(this);
    dock_pyterm->setWidget(pyterm);

    /* Connect templateeditor and the main window */
    connect(this, SIGNAL(fileFocused(mc_file_t*)),
            pyterm, SLOT(setFile(mc_file_t*)));

    /* Add dock to window */
    addDockWidget(Qt::BottomDockWidgetArea, dock_pyterm);
}

bool Mycroft::createWindowToggleMenu()
{
    menu_dockable_widgets = ui->menu_window->addMenu("Dockable Widgets");

    /* Add action for projectview */
    dock_projectview->toggleViewAction()->setText("Project View");
    menu_dockable_widgets->addAction(dock_projectview->toggleViewAction());

    /* Add action for projectview */
    dock_templateeditor->toggleViewAction()->setText("Template Editor");
    menu_dockable_widgets->addAction(dock_templateeditor->toggleViewAction());

    /* Add action for projectview */
    dock_pyterm->toggleViewAction()->setText("Python Terminal");
    menu_dockable_widgets->addAction(dock_pyterm->toggleViewAction());
}

bool Mycroft::destroyMainEditor()
{

}

bool Mycroft::destroyTemplateEditor()
{

}

bool Mycroft::destroyProjectView()
{

}

bool Mycroft::destroyPythonTerminal()
{

}

bool Mycroft::destroyWindowToggleMenu()
{

}

bool Mycroft::openFile(QString filename)
{
    mc_file_t* file = NULL;
    mc_project_t* project = NULL;

    /* Use focused project or make a new one */
    project = mc_get_focused_project();
    if (project == NULL) {

        /* Create project */
        project = mc_project_create(filename.toStdString().c_str());
        if (project == NULL) {
            MC_ERROR("Failed to allocate new project.\n");
            return false;
        }

        /* Try adding it */
        if (!addProject(project)) {
            MC_ERROR("Failed to add new project.\n");
            return false;
        }
    }

    /* */
    file = mc_file_open(filename.toStdString().c_str());
    if (file == NULL) {
        MC_ERROR("Failed to open file.\n");
        return false;
    }

    emit addFile(project, file);

    return true;
}

bool Mycroft::openProject(QString projectname)
{
    mc_project_t* project = NULL;

    /* Create project */
    project = mc_project_create(projectname.toStdString().c_str());
    if (project == NULL) {
        MC_ERROR("Failed to allocate new project.\n");
        return false;
    }

    emit addProject(project);

    return true;
}

bool Mycroft::openTemplate(QString file)
{
    return false;
}

void Mycroft::on_action_file_open()
{
    QString filters("Mycroft Project File (*.mpf);;All files (*)");
    QString defaultFilter("All files (*)");

    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open File",
        QDir::currentPath(),
        filters,
        &defaultFilter);
    QFileInfo fi(file);

    if (fi.completeSuffix() == MYCROFT_TEMPLATE_EXTENSION) {
        openTemplate(file);
    }
    else if (fi.completeSuffix() == MYCROFT_PROJECT_EXTENSION) {
        openProject(file);
    }
    else {
        openFile(file);
    }
}

void Mycroft::on_action_file_exit()
{
    this->close();
}

void Mycroft::on_action_undo()
{
    MC_DEBUG("on_action_undo\n");
}

void Mycroft::on_action_redo()
{
    MC_DEBUG("on_action_redo\n");
}

void Mycroft::on_action_cut()
{
    MC_DEBUG("on_action_cut\n");
}

void Mycroft::on_action_copy()
{
    MC_DEBUG("on_action_copy\n");
}

void Mycroft::on_action_paste()
{
    MC_DEBUG("on_action_paste\n");
}

void Mycroft::on_action_delete()
{
    MC_DEBUG("on_action_delete\n");
}

void Mycroft::on_action_selectall()
{
    MC_DEBUG("on_action_selectall\n");
}

void Mycroft::on_action_script_run_file()
{
    QString filters("Python Scripts (*.py);;All files (*)");
    QString defaultFilter("Python Scripts (*.py)");

    /* Static method approach */
    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open Script File",
        QDir::currentPath(),
        filters,
        &defaultFilter);

    MC_DEBUG("Running script: \"%s\"\n", file.toStdString().c_str());

    if (mc_script_runfile(file.toStdString().c_str()) != MC_OK) {
        MC_ERROR("Failed to run script.\n");
    }
}

void Mycroft::on_action_project_new()
{
    MC_DEBUG("on_action_project_new\n");
}

void Mycroft::on_action_project_open()
{
    QString filters("Mycroft Project File (*.mpf);;All files (*)");
    QString defaultFilter("Mycroft Project File (*.mpf)");

    /* Static method approach */
    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open Project File",
        QDir::currentPath(),
        filters,
        &defaultFilter);

    printf("Opening template: \"%s\"\n", file.toStdString().c_str());
    //int rc = mc_plugin_runfile(file.toStdString().c_str());
    //if (rc < 0) {
    //    printf("Failed to run script!\n");
    //}
}

void Mycroft::on_action_project_saveas()
{
    QFileDialog fd(this, "Choose file to save");
    fd.setNameFilter("Mycroft Project File (*.mpf)");
    fd.setDefaultSuffix("mpf");
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.exec();
    QString file = fd.selectedFiles().first();

    printf("Saving project file: \"%s\"\n", file.toStdString().c_str());
    if (mc_get_focused_project()) {
        if (mc_project_save(mc_get_focused_project(), file.toStdString().c_str())) {

        }
    }
    else {
        QMessageBox::information(this, "Failed to save project", "No project currently in focus.", QMessageBox::Ok);
    }
    //int rc = mc_plugin_runfile(file.toStdString().c_str());
    //if (rc < 0) {
    //    printf("Failed to run script!\n");
    //}
}

void Mycroft::on_action_template_open()
{
    QString filters("Mycroft Template File (*.mtf);;All files (*)");
    QString defaultFilter("Mycroft Template File (*.mtf)");

    /* Static method approach */
    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open Script File",
        QDir::currentPath(),
        filters,
        &defaultFilter);

    printf("Opening template: \"%s\"\n", file.toStdString().c_str());
    //int rc = mc_plugin_runfile(file.toStdString().c_str());
    //if (rc < 0) {
    //    printf("Failed to run script!\n");
    //}
}

bool Mycroft::addFile(mc_file_t* file)
{
    mc_project_t* proj = mc_get_focused_project();
    if (proj == NULL) {
        MC_ERROR("No focused project; where do we add this file??\n");
        return false;
    }

    return addFile(proj, file);
}

bool Mycroft::addFile(mc_project_t* proj, mc_file_t* file)
{
    /* Add file to project and notify others */
    if (mc_project_add_file(proj, file) != MC_OK) {
        MC_ERROR("Failed to add file to mc_project_t.\n");
        return false;
    }
    emit fileAdded(proj, file);

    /* Focus the file */
    if (!setFocusedFile(proj, file)) {
        return false;
    }

    return true;
}

bool Mycroft::removeFile(mc_file_t* file)
{
    mc_project_t* proj = mc_get_focused_project();
    if (proj == NULL) {
        MC_ERROR("No focused project; where do we add this file??\n");
        return false;
    }

    return removeFile(proj, file);
}

bool Mycroft::removeFile(mc_project_t* proj, mc_file_t* file)
{
    /* If we currently have the given file focused, let everyone know we
       unfocused it */
    if (file == mc_project_get_focused_file(proj)) {
        emit fileFocused(NULL);
    }

    /* Remove file from project and notify others */
    //if (mc_project_remove_file(proj, mc_project_get_file_index(file)) != MC_OK) {
    //    return false;
    //}
    //emit fileRemoved(proj, file);

    return true;
}

bool Mycroft::setFocusedFile(mc_project_t* proj, mc_file_t* file)
{
    mc_project_set_focused_file(proj, file);
    emit fileFocused(file);
    return true;
}

bool Mycroft::addProject(mc_project_t* proj)
{
    /* Add project to mycroft and notify others */
    if (mc_add_project(proj) != MC_OK) {
        MC_ERROR("Failed to add project to mycroft.\n");
        return false;
    }
    emit projectAdded(proj);

    /* Focus the project */
    if (!setFocusedProject(proj)) {
        return false;
    }

    return true;
}

bool Mycroft::removeProject(mc_project_t* proj)
{
    /* If we currently have the given project focused, let everyone know we
       unfocused it */
    if (proj == mc_get_focused_project()) {
        emit fileFocused(NULL);
    }

    /* Remove project from ctx and notify others */
    //if (mc_project_remove_file(proj, mc_project_get_file_index(file)) != MC_OK) {
    //    return false;
    //}
    //emit fileRemoved(proj, file);

    return true;
}

bool Mycroft::setFocusedProject(mc_project_t* proj)
{
    mc_set_focused_project(proj);
    emit projectFocused(proj);
    return true;
}
