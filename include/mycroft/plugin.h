#ifndef MYCROFT_PLUGIN_H
#define MYCROFT_PLUGIN_H

#include <mycroft/context.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mc_plugin_entry_t mc_plugin_entry_t;

int mc_plugin_init(mc_ctx_t* ctx);
int mc_plugin_close(mc_ctx_t* ctx);

int mc_plugin_run_plugins(mc_ctx_t* ctx);
int mc_plugin_addpath(mc_ctx_t* ctx, const char* path);

int mc_plugin_load(mc_ctx_t* ctx, const char* path);
int mc_plugin_unload(mc_ctx_t* ctx, const char* path);
int mc_plugin_getloaded(mc_ctx_t* ctx, const char* path);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PLUGIN_H
