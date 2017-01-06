#ifndef MYCROFT_SCRIPT_H
#define MYCROFT_SCRIPT_H

#include <mycroft/error.h>
#include <mycroft/context.h>

mc_error_t mc_script_runfile(mc_ctx_t* ctx, const char* path);
mc_error_t mc_script_runstring(const char* script);

#endif // MYCROFT_SCRIPT_H
