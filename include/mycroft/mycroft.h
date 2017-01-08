#ifndef MYCROFT_H
#define MYCROFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/project.h>

/** Expected file extension for a mycroft template file */
#define MYCROFT_TEMPLATE_EXTENSION "mtf"

/** Expected file extension for a mycroft project file */
#define MYCROFT_PROJECT_EXTENSION "mpf"

/* Create/free */
mc_error_t mc_init();
void mc_destroy();

/* Context management */
mc_error_t mc_add_project(mc_project_t* project);
mc_error_t mc_remove_project(uint32_t project_index);
mc_project_t* mc_get_project(uint32_t project_index);
uint32_t mc_get_project_amount();
mc_error_t mc_set_focused_project(mc_project_t* project);
mc_project_t* mc_get_focused_project();

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_H
