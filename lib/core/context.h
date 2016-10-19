#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mycroft.h>
#include <mycroft/project.h>
#include <mycroft/mdb.h>

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
