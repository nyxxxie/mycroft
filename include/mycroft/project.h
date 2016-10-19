#ifndef MYCROFT_PROJECT_H
#define MYCROFT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/file.h>

typedef struct mc_project_t mc_project_t;

/* Create/free */
mc_project_t* mc_project_create(const char* name);
mc_project_t* mc_project_load(const char* mdb_file);
void mc_project_free(mc_project_t* project);

/* File management */
uint32_t mc_project_add_file(mc_project_t* project, mc_file_t* file);
int mc_project_remove_file(mc_project_t* project, uint32_t file_index);
mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index);
uint32_t mc_project_get_file_amount(mc_project_t* project);
void mc_project_set_focused_file(mc_project_t* project, mc_file_t* file);
mc_file_t* mc_project_get_focused_file(mc_project_t* project);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PROJECT_H