#include <mycroft/context.h>
#include "mainhexeditor.h"
#include "mycroft.h"
#include "ui_mycroft.h"

Mycroft::Mycroft(mc_ctx_t* ctx, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Mycroft)
{
    ui->setupUi(this);

    editor = new MainHexEditor(this);
    ui->windowlayout->addWidget(editor);

    setContext(ctx);
}

Mycroft::~Mycroft()
{
    delete ui;
}

void Mycroft::setContext(mc_ctx_t* ctx)
{
    this->ctx = ctx;
    emit contextChanged(ctx);
}

bool Mycroft::addFile(mc_file_t* file)
{
    /* Check to make sure we have a usable ctx */
    if (ctx == NULL) {
        MC_ERROR("Main window has a bad ctx.\n");
        return false;
    }

    mc_project_t* proj = mc_ctx_get_focused_project(ctx);
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

    /* Set the file we just added to be focused and notfiy others */
    mc_project_set_focused_file(proj, file);
    emit fileFocused(file);

    return true;
}

bool Mycroft::removeFile(mc_file_t* file)
{
    /* Check to make sure we have a usable ctx */
    if (ctx == NULL) {
        MC_ERROR("Main window has a bad ctx.\n");
        return false;
    }

    mc_project_t* proj = mc_ctx_get_focused_project(ctx);
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

bool Mycroft::addProject(mc_project_t* proj)
{
    /* Make sure ctx is good */
    if (ctx == NULL) {
        MC_ERROR("Main window has a bad ctx.\n");
        return false;
    }

    /* Add project to ctx and notify others */
    if (mc_ctx_add_project(ctx, proj) != MC_OK) {
        MC_ERROR("Failed to add project to mc_ctx_t.\n");
        return false;
    }
    emit projectAdded(proj);

    /* Set the project we just added to be focused and notfiy others */
    mc_ctx_set_focused_project(ctx, proj);
    emit projectFocused(proj);

    return true;
}

bool Mycroft::removeProject(mc_project_t* proj)
{
    /* Make sure ctx is good */
    if (ctx == NULL) {
        MC_ERROR("Main window has a bad ctx.\n");
        return false;
    }

    /* If we currently have the given project focused, let everyone know we
       unfocused it */
    if (proj == mc_ctx_get_focused_project(ctx)) {
        emit fileFocused(NULL);
    }

    /* Remove project from ctx and notify others */
    //if (mc_project_remove_file(proj, mc_project_get_file_index(file)) != MC_OK) {
    //    return false;
    //}
    //emit fileRemoved(proj, file);

    return true;
}
