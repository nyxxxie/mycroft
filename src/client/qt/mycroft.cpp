#include <QFileDialog>
#include <mycroft/mycroft.h>
#include <mycroft/error.h>
#include "mycroft.h"
#include "mainhexeditor.h"
#include "ui_mycroft.h"

Mycroft::Mycroft(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Mycroft)
{
    ui->setupUi(this);

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
    connect(ui->action_template_open, SIGNAL(triggered()), this, SLOT(on_action_template_open()));

    editor = new MainHexEditor(this);
    ui->windowlayout->addWidget(editor);
}

Mycroft::~Mycroft()
{
    delete ui;
    delete editor;
}

void Mycroft::on_action_file_open()
{
    QString filters("Mycroft Template File (*.mtf);;All files (*.*)");
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
    QString filters("Python Scripts (*.py);;All files (*.*)");
    QString defaultFilter("Python Scripts (*.py)");

    /* Static method approach */
    QString file = QFileDialog::getOpenFileName(
        NULL,
        "Open Script File",
        QDir::currentPath(),
        filters,
        &defaultFilter);

    printf("Running script: \"%s\"\n", file.toStdString().c_str());
    //int rc = mc_plugin_runfile(file.toStdString().c_str());
    //if (rc < 0) {
    //    printf("Failed to run script!\n");
    //}
}

void Mycroft::on_action_project_new()
{
    MC_DEBUG("on_action_project_new\n");
}

void Mycroft::on_action_project_open()
{
    QString filters("Mycroft Project File (*.mpf);;All files (*.*)");
    QString defaultFilter("Mycroft Project File (*.mpf)");

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

void Mycroft::on_action_template_open()
{
    QString filters("Mycroft Template File (*.mtf);;All files (*.*)");
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
