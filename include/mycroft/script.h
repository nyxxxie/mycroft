#ifndef MYCROFT_SCRIPT_H
#define MYCROFT_SCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/error.h>
#include <mycroft/context.h>

typedef struct mc_interpreter_t mc_interpreter_t;

mc_interpreter_t* mc_interpreter_create();
void mc_interpreter_free(mc_interpreter_t* i);

mc_error_t mc_script_runfile(mc_interpreter_t* i, const char* path);
mc_error_t mc_script_runstring(mc_interpreter_t* i, const char* script);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_SCRIPT_H
