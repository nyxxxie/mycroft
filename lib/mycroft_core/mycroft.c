#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mycroft/mycroft.h>
#include "mycroft.h"
#include "interpreter.h"
#include "project.h"
#include "plugin.h"


mc_ctx_t* _ctx = NULL;

mc_error_t mc_init() {

    /* Allocate space for the ctx */
    _ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (_ctx == NULL) {
        return MC_ERR;
    }

    /* Set each field to default value */
    _ctx->projects = NULL;
    _ctx->project_amt = 0;
    _ctx->project_focused = NULL;
    _ctx->interpreter = NULL;

    /* */
    _ctx->interpreter = mc_interpreter_create();
    if (_ctx->interpreter == NULL) {
        MC_ERROR("Failed to create interpreter.\n");
        return MC_ERR;
    }

    return MC_OK;
}

void mc_destroy() {
    if (_ctx != NULL) {
        if (_ctx->projects != NULL) {
            mc_project_t* project = NULL;
            int i = 0;

            for (; i < _ctx->project_amt; i++) {
                mc_project_free(_ctx->projects[i]);
            }

            free(_ctx->projects);
        }
        if (_ctx->interpreter) {
            free(_ctx->interpreter);
        }

        free(_ctx);
    }
}

mc_error_t mc_add_project(mc_project_t* project) {
    mc_error_t rc = MC_ERR;
    uint32_t cur_index = 0;
    uint32_t i = 0;

    /* Ensure that file isn't null */
    if (project == NULL) {
        MC_ERROR("Project input is null.\n");
        return rc;
    }

    /* Ensure we haven't already added the file */
    for (i=0; i < _ctx->project_amt; i++) {
        mc_project_t* curp = _ctx->projects[i];
        if (curp == project ||
            strcmp(curp->name, project->name) == 0) {
            MC_ERROR("Input project already has been added to context.\n");
            return rc;
        }
    }

    /* Save index that the project should be saved to, then iterate total */
    cur_index = _ctx->project_amt;
    _ctx->project_amt++;

    /* Create more space in the project array */
    _ctx->projects = realloc(_ctx->projects, _ctx->project_amt);
    if (_ctx->projects == NULL) {
        MC_ERROR("Failed to (re)allocate project array.\n");
        return rc;
    }

    /* Add project to the array */
    _ctx->projects[cur_index] = project;

    /* Focus project */
    _ctx->project_focused = project;

    return MC_OK;
}

mc_error_t mc_remove_project(uint32_t project_index) {

    /* Ensure that the index is valid */
    if (project_index >= _ctx->project_amt) {
        return MC_ERR;
    }

    /* If this operation will remove the last file, just delete the array */
    if (_ctx->project_amt == 1) {
        free(_ctx->projects);
        _ctx->projects = NULL;
        _ctx->project_amt = 0;
        return MC_OK;
    }

    /* Shift contents of memory over to replace removed element */
    memmove(&_ctx->projects[project_index], &_ctx->projects[project_index+1],
        (_ctx->project_amt-project_index) * sizeof(_ctx->projects[0]));


    /* Decrement the amount of files we're tracking. */
    _ctx->project_amt--;

    /* Create more space in the file array */
    _ctx->projects = realloc(_ctx->projects, _ctx->project_amt);
    if (_ctx->projects == NULL) {
        return MC_ERR;
    }

    return MC_OK;
}

mc_project_t* mc_get_project(uint32_t project_index) {
    if (_ctx->projects == NULL ||
        project_index >= _ctx->project_amt) {
        return NULL;
    }

    return _ctx->projects[project_index];
}

uint32_t mc_get_project_amount() {
    return _ctx->project_amt;
}

mc_error_t mc_set_focused_project(mc_project_t* project) {
    uint32_t i = 0;

    /* Ensure we added the file */
    for (i=0; i < _ctx->project_amt; i++) {
        if (project == _ctx->projects[i]) {
            /* We've found the file, focus it and return */
            _ctx->project_focused = project;
            return MC_OK;
        }
    }

    /* We didn't find the project */
    MC_ERROR("Failed to find project in internal list, did you add it with mc_add_project()?\n");
    return MC_ERR;
}

mc_project_t* mc_get_focused_project() {
    return _ctx->project_focused;
}
