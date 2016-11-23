#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/context.h>
#include <mycroft/project.h>

/** Represents the state of a file opened in mycroft */
struct mc_ctx_t {
    mc_project_t** projects;
    uint32_t       project_amt;
    mc_project_t*  project_focused;
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_CORE_H
