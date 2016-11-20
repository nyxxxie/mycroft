#ifndef MYCROFT_CONTEXT_H
#define MYCROFT_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mycroft/error.h>
#include <mycroft/context.h>
#include <mycroft/project.h>

typedef struct mc_ctx_t mc_ctx_t;

/* Create/free */
mc_ctx_t* mc_ctx_create();
void mc_ctx_free(mc_ctx_t* ctx);

/* Context management */
mc_error_t mc_ctx_add_project(mc_ctx_t* ctx, mc_project_t* project);
mc_error_t mc_ctx_remove_project(mc_ctx_t* ctx, uint32_t project_index);
mc_project_t* mc_ctx_get_project(mc_ctx_t* ctx, uint32_t project_index);
uint32_t mc_ctx_get_project_amount(mc_ctx_t* ctx);
mc_error_t mc_ctx_set_focused_project(mc_ctx_t* ctx, mc_project_t* project);
mc_project_t* mc_ctx_get_focused_project(mc_ctx_t* ctx);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_CONTEXT_H
