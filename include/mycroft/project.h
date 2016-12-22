#ifndef MYCROFT_PROJECT_H
#define MYCROFT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/file.h>
#include <mycroft/error.h>

typedef struct mc_project_t mc_project_t;

/* Create/free */
mc_project_t* mc_project_create(const char* name);
mc_project_t* mc_project_load(const char* mdb_file);
mc_error_t mc_project_save(mc_project_t* project, const char* mdb_file);
void mc_project_free(mc_project_t* project);

/* Project info */
mc_error_t mc_project_set_name(mc_project_t* project, const char* name);
char* mc_project_get_name(mc_project_t* project);

/* File management */
mc_error_t mc_project_add_file(mc_project_t* project, mc_file_t* file);
mc_error_t mc_project_remove_file(mc_project_t* project, uint32_t file_index);
mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index);
uint32_t mc_project_get_file_amount(mc_project_t* project);
mc_error_t mc_project_set_focused_file(mc_project_t* project, mc_file_t* file);
mc_file_t* mc_project_get_focused_file(mc_project_t* project);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PROJECT_H
