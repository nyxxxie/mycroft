#ifndef MYCROFT_SCRIPT_H
#define MYCROFT_SCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/error.h>
#include <mycroft/context.h>

mc_error_t mc_script_runfile(mc_ctx_t* ctx, const char* path);
mc_error_t mc_script_runstring(mc_ctx_t* ctx, const char* script);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_SCRIPT_H
