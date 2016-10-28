#include <stdlib.h>
#include <string.h>
#include "project.h"
#include "file.h"

#define QUERYNO 3
int create_default_tables(sqlite3* db) {
    const char* create_queries[QUERYNO] = {
        /* This table stores project information like name, date created, date
         * last modified, etc... */
        "create table project_info("
        "key       text  unique,"
        "value     blob);",

        /* This table stores the files opened by this project.  Please note
         * the file attribute is the name of the file (including extension),
         * whereas path contains the full path to the file. */
        "create table files("
        "file      text"
        "path      text);",

        /* This stores generic metadata about a file.  More detailed metadata
         * can be found in other tables using the datatype specified (for
         * example, comments) . */
        "create table metadata("
        "byte_num  integer"
        "datatype  integer"
        "file      text);",
    };

    // TODO: make function that creates a key/value table (so we can make more
    //       easily for testing

    for (int i = 0; i < QUERYNO; i++) {
        sqlite3_stmt* stmt;
        int rc = 0;

        if ((rc = sqlite3_prepare_v2(db,
            create_queries[i], -1,
            &stmt, NULL)) != SQLITE_OK) {
//            fprintf(stderr, "asdf: %s\n",
//                sqlite3_errmsg(mdb->db));

            return -1;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
//            fprintf(stderr, "asdf: %s\n",
//                sqlite3_errmsg(mdb->db));

            return -1;
        }

        rc = sqlite3_finalize(stmt);
        if (rc != SQLITE_OK) {
//            fprintf(stderr, "asdf: %s\n",
//                sqlite3_errmsg(mdb->db));

            return -1;
        }
    }

    return 0;
}

mc_project_t* mc_project_create(const char* name) {
    mc_project_t* project = NULL;
    int rc = 0;
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

    /* Create in-memory sqlite database */
    rc = sqlite3_open_v2(":memory:", &project->db,
        SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        return -1;
    }

    /* Create default tables for this project */
    rc = create_default_tables(project->db);
    if (rc < 0) {
        return -1;
    }

    return project;
}

mc_project_t* mc_project_load(const char* mdb_file) {
    mc_project_t* project = NULL;

    /* Make sure our target database exists */
    if (file_exists(mdb_file) < 0) {
        return NULL;
    }

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

    /* Ensure that file isn't null */
    if (file == NULL) {
        return (uint32_t)(-1);
    }

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

    /* Ensure that the index is valid */
    if (file_index >= project->file_amt) {
        return (uint32_t)(-1);
    }

    /* If this operation will remove the last file, just delete the array */
    if (project->file_amt == 1) {
        free(project->files);
        project->files = NULL;
        project->file_amt = 0;
        return 0;
    }

    /* Shift contents of memory over to replace removed element */
    memmove(&project->files[file_index], &project->files[file_index+1],
        (project->file_amt-file_index) * sizeof(project->files[0]));


    /* Decrement the amount of files we're tracking. */
    project->file_amt--;

    /* Create more space in the file array */
    project->files = realloc(project->files, project->file_amt);
    if (project->files == NULL) {
        return (uint32_t)(-1);
    }

    return 0;
}

mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index) {

    if (project->files == NULL ||
        file_index >= project->file_amt) {
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
