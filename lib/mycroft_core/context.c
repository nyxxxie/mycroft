#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "context.h"
#include "project.h"
#include "plugin.h"
#include "database.h"

mc_ctx_t* mc_ctx_create() {
    mc_ctx_t* ctx = NULL;

    /* Allocate space for the ctx */
    ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }

    /* Set each field to default value */
    ctx->projects = NULL;
    ctx->project_amt = 0;
    ctx->project_focused = NULL;

    return ctx;
}

void mc_ctx_free(mc_ctx_t* ctx) {
    if (ctx != NULL) {
        if (ctx->projects != NULL) {
            mc_project_t* project = NULL;
            int i = 0;

            for (; i < ctx->project_amt; i++) {
                mc_project_free(ctx->projects[i]);
            }

            free(ctx->projects);
        }

        free(ctx);
    }
}

mc_error_t mc_ctx_add_project(mc_ctx_t* ctx, mc_project_t* project) {
    mc_error_t rc = MC_ERR;
    uint32_t cur_index = 0;
    uint32_t i = 0;

    /* Ensure that file isn't null */
    if (project == NULL) {
        MC_ERROR("Project input is null.");
        return rc;
    }

    /* Ensure we haven't already added the file */
    for (i=0; i < ctx->project_amt; i++) {
        mc_project_t* curp = ctx->projects[i];
        if (curp == project ||
            strcmp(curp->name, project->name) == 0) {
            MC_ERROR("Input project already has been added to context.\n");
            return rc;
        }
    }

    /* Save index that the project should be saved to, then iterate total */
    cur_index = ctx->project_amt;
    ctx->project_amt++;

    /* Create more space in the project array */
    ctx->projects = realloc(ctx->projects, ctx->project_amt);
    if (ctx->projects == NULL) {
        MC_ERROR("Failed to (re)allocate project array.\n");
        return rc;
    }

    /* Add project to the array */
    ctx->projects[cur_index] = project;

    /* Focus project */
    ctx->project_focused = project;

    return MC_OK;
}

mc_error_t mc_ctx_remove_project(mc_ctx_t* ctx, uint32_t project_index) {

    /* Ensure that the index is valid */
    if (project_index >= ctx->project_amt) {
        return MC_ERR;
    }

    /* If this operation will remove the last file, just delete the array */
    if (ctx->project_amt == 1) {
        free(ctx->projects);
        ctx->projects = NULL;
        ctx->project_amt = 0;
        return MC_OK;
    }

    /* Shift contents of memory over to replace removed element */
    memmove(&ctx->projects[project_index], &ctx->projects[project_index+1],
        (ctx->project_amt-project_index) * sizeof(ctx->projects[0]));


    /* Decrement the amount of files we're tracking. */
    ctx->project_amt--;

    /* Create more space in the file array */
    ctx->projects = realloc(ctx->projects, ctx->project_amt);
    if (ctx->projects == NULL) {
        return MC_ERR;
    }

    return MC_OK;
}

mc_project_t* mc_ctx_get_project(mc_ctx_t* ctx, uint32_t project_index) {
    if (ctx->projects == NULL ||
        project_index >= ctx->project_amt) {
        return NULL;
    }

    return ctx->projects[project_index];
}

uint32_t mc_ctx_get_project_amount(mc_ctx_t* ctx) {
    return ctx->project_amt;
}

mc_error_t mc_ctx_set_focused_project(mc_ctx_t* ctx, mc_project_t* project) {
    uint32_t i = 0;

    /* Ensure we added the file */
    for (i=0; i < project->file_amt; i++) {
        if (project == ctx->projects[i]) {
            /* We've found the file, focus it and return */
            ctx->project_focused = project;
            return MC_OK;
        }
    }

    /* We didn't find the file */
    return MC_ERR;
}

mc_project_t* mc_ctx_get_focused_project(mc_ctx_t* ctx) {
    return ctx->project_focused;
}
