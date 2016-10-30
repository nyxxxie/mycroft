#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "context.h"
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

uint32_t mc_ctx_add_project(mc_ctx_t* ctx, mc_project_t* project) {
    int cur_index = 0;

    /* Ensure that file isn't null */
    if (project == NULL) {
        return (uint32_t)(-1);
    }

    /* Save index that the project should be saved to, then iterate total */
    cur_index = ctx->project_amt;
    ctx->project_amt++;

    /* Create more space in the project array */
    ctx->projects = realloc(ctx->projects, ctx->project_amt);
    if (ctx->projects == NULL) {
        return (uint32_t)(-1);
    }

    /* Add project to the array */
    ctx->projects[cur_index] = project;

    return cur_index;
}

int mc_ctx_remove_project(mc_ctx_t* ctx, uint32_t project_index) {

    /* Ensure that the index is valid */
    if (project_index >= ctx->project_amt) {
        return (uint32_t)(-1);
    }

    /* If this operation will remove the last file, just delete the array */
    if (ctx->project_amt == 1) {
        free(ctx->projects);
        ctx->projects = NULL;
        ctx->project_amt = 0;
        return 0;
    }

    /* Shift contents of memory over to replace removed element */
    memmove(&ctx->projects[project_index], &ctx->projects[project_index+1],
        (ctx->project_amt-project_index) * sizeof(ctx->projects[0]));


    /* Decrement the amount of files we're tracking. */
    ctx->project_amt--;

    /* Create more space in the file array */
    ctx->projects = realloc(ctx->projects, ctx->project_amt);
    if (ctx->projects == NULL) {
        return (uint32_t)(-1);
    }

    return 0;
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

void mc_ctx_set_focused_project(mc_ctx_t* ctx, mc_project_t* project) {
    ctx->project_focused = project;
}

mc_project_t* mc_ctx_get_focused_project(mc_ctx_t* ctx) {
    return ctx->project_focused;
}
