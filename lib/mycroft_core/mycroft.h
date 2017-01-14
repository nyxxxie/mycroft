#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mycroft.h>
#include <mycroft/project.h>
#include "interpreter.h"
#include "typesystem/typesystem.h"

/** @internal Contains global data for mycroft.  Meant to be accessed using mc functions, don't access this yourself. */
typedef struct {
    mc_project_t**    projects;
    uint32_t          project_amt;
    mc_project_t*     project_focused;
    mc_interpreter_t* interpreter;
    mc_typesystem_t*  typesystem;
} mc_ctx_t;

/** @internal Global internal ctx instance.  Don't use this directly outside of mycroft.c! */
extern mc_ctx_t* _ctx;

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_CORE_H
