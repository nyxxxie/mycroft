#ifndef MYCROFT_INT_SCRIPT_H
#define MYCROFT_INT_SCRIPT_H

#include <mycroft/script.h>

#define INITSCRIPT "init.py"

/**
 * Represents a script
 */
struct mc_interpreter_t {
    int bindings_initialized;
};

#endif // MYCROFT_INT_SCRIPT_H
