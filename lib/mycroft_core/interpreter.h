#ifndef MYCROFT_INT_SCRIPT_H
#define MYCROFT_INT_SCRIPT_H

#include <mycroft/error.h>

/**
 * Represents a script
 */
typedef struct {
    int bindings_initialized;
} mc_interpreter_t;

mc_interpreter_t* mc_interpreter_create();
void mc_interpreter_free(mc_interpreter_t* i);

mc_error_t mc_interpreter_add_path(mc_interpreter_t* i, const char* directory);

/** Global interpreter instance.  This is global because there can only be one python intepreter instance. */
extern mc_interpreter_t* _interpreter;
#define MC_INTERPRETER_START \
    if (_interpreter == NULL) { \
        _interpreter = mc_interpreter_create(); \
    }

#define MC_INTERPRETER_END \
    if (_interpreter != NULL) { \
        mc_interpreter_free(_interpreter); \
    }

#endif // MYCROFT_INT_SCRIPT_H
