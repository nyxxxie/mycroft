#include <stdlib.h>
#include <string.h>
#include "project.h"
#include "file.h"

mc_project_t* mc_project_create(const char* name) {
    mc_project_t* project = NULL;
    int name_len = 0;

    /* Alloc the project */
    project = (mc_project_t*)malloc(sizeof(mc_project_t));
    if (project == NULL) {
        return NULL;
    }

    /* Set each field to default value */
    project->files = NULL;
    project->file_amt = 0;
    project->file_focused = NULL;

    /* Check if name field is null or an empty string */
    if (name != NULL) {
        name_len = strlen(name); // Saving this so we can use it below
        if (name_len == 0) {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    /* Copy name argument into name field */
    project->name = (char*)malloc(name_len + 1);
    if (project->name == NULL) {
        return NULL;
    }
    strncpy(project->name, name, name_len);

    return project;
}

mc_project_t* mc_project_load(const char* mdb_file) {
    mc_project_t* project = NULL;

    /* Alloc the project */
    project = (mc_project_t*)malloc(sizeof(mc_project_t));
    if (project == NULL) {
        return NULL;
    }

    // TODO: open database and read project values, open files, etc

    return NULL;
}

void mc_project_free(mc_project_t* project) {
    if (project != NULL) {
        if (project->name != NULL) {
            free(project->name);
        }

        if (project->files != NULL) {
            mc_file_t* file = NULL;
            int i = 0;

            for (; i < project->file_amt; i++) {
                mc_file_close(project->files[i]);
            }

            free(project->files);
        }

        free(project);
    }
}

uint32_t mc_project_add_file(mc_project_t* project, mc_file_t* file) {
    int cur_index = 0;

    /* Save index that the file should be saved to, then iterate total */
    cur_index = project->file_amt;
    project->file_amt++;

    /* Create more space in the file array */
    project->files = realloc(project->files, project->file_amt);
    if (project->files == NULL) {
        return (uint32_t)(-1);
    }

    /* Add file to the array */
    project->files[cur_index] = file;

    return cur_index;
}

int mc_project_remove_file(mc_project_t* project, uint32_t file_index) {

    // TODO: implement

    return 0;
}

mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index) {

    if (project->files) {
        return NULL;
    }

    return project->files[file_index];
}

uint32_t mc_project_get_file_amount(mc_project_t* project) {
    return project->file_amt;
}

void mc_project_set_focused_file(mc_project_t* project, mc_file_t* file) {
    project->file_focused = file;
}

mc_file_t* mc_project_get_focused_file(mc_project_t* project) {
    return project->file_focused;
}
