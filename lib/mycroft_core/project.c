#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "project.h"
#include "file.h"

/** @internal Number of initialization queries to run */
#define QUERYNO 3

/**
 * Creates the tables in a newly created mycroft database.
 *
 * @param db sqlite3 database to operate on.
 * @return mc_error_t code indicating success or failure.
 * @internal
 */
mc_error_t create_default_tables(sqlite3* db) {
    const char* create_queries[QUERYNO] = {
        /* This table stores project information like name, date created, date
           last modified, etc... */
        "create table project_info("
        "key       text  unique,"
        "value     blob);",

        /* This table stores the files opened by this project.  Please note
           the file attribute is the name of the file (including extension),
           whereas path contains the full path to the file. */
        "create table files("
        "file      text  unique,"
        "path      text);",

        /* This stores generic metadata about a file.  More detailed metadata
           can be found in other tables using the datatype specified (for
           example, comments) . */
        "create table metadata("
        "byte_num  integer,"
        "datatype  integer,"
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
            MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
            return MC_ERR;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
            return MC_ERR;
        }

        rc = sqlite3_finalize(stmt);
        if (rc != SQLITE_OK) {
            MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
            return MC_ERR;
        }
    }

    return MC_OK;
}

/**
 * Create new project with a given name.
 *
 * @param name Name for new project
 * @return Pointer to mc_project_t that was just created.
 */
mc_project_t* mc_project_create(const char* name) {
    mc_project_t* project = NULL;
    int rc = 0;

    /* Alloc the project */
    project = (mc_project_t*)malloc(sizeof(mc_project_t));
    if (project == NULL) {
        return NULL;
    }

    /* Set each field to default value */
    project->files = NULL;
    project->file_amt = 0;
    project->file_focused = NULL;
    project->name = NULL;
    project->db = NULL;

    /* Set file name */
    if (mc_project_set_name(project, name) == MC_ERR) {
        return NULL;
    }

    /* Create in-memory sqlite database */
    rc = sqlite3_open_v2(":memory:", &project->db,
        SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }

    /* Create default tables for this project */
    if (create_default_tables(project->db) != MC_OK) {
        return NULL;
    }

    return project;
}

/**
 * Loads project from an mdb file.
 *
 * @param mdb_file Path to mdb file to load.
 * @return Pointer to mc_project_t that was loaded.
 */
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

/**
 * Saves project to a file.
 *
 * @param project mc_project_t to save.
 * @param mdb_file File to save to.  Can be a file that exists or doesn't.
 * @return mc_error_t indicating success or failure.
 */
mc_error_t mc_project_save(mc_project_t* project, const char* mdb_file) {
    int rc = 0;
    sqlite3* savefile = NULL;
    sqlite3_backup* backup = NULL;

    /* Open the database to save to */
    rc = sqlite3_open(mdb_file, &savefile);
    if (rc == SQLITE_OK) {

        /* */
        backup = sqlite3_backup_init(savefile, "main", project->db, "main");
        if (backup == NULL) {
            return MC_ERR;
        }
        else {
            sqlite3_backup_step(backup, -1);
            sqlite3_backup_finish(backup);
        }

        /* */
        rc = sqlite3_errcode(savefile);
        if (rc != SQLITE_OK) {
            return MC_ERR;
        }
    }

    sqlite3_close(savefile);

    return MC_OK;
}

/**
 * Free project.  This MUST be called when you are done with an mc_project_t.
 *
 * @param project mc_project_t to free.
 */
void mc_project_free(mc_project_t* project) {
    if (project != NULL) {
        /* Free file name */
        if (project->name != NULL) {
            free(project->name);
        }

        /* Close database */
        if (project->db != NULL) {
            sqlite3_close(project->db);
        }

        /* Close all files we've opened */
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


mc_error_t mc_project_set_name(mc_project_t* project, const char* name) {
    int name_len = 0;

    /* Check if name is null or an empty string */
    if (name != NULL) {
        name_len = strlen(name); // Saving this so we can use it below
        if (name_len == 0) {
            return MC_ERR;
        }
    }
    else {
        return MC_ERR;
    }

    /* Copy name argument into name field */
    project->name = (char*)malloc(name_len + 1);
    if (project->name == NULL) {
        return MC_ERR;
    }
    memset(project->name, 0, name_len + 1);
    strncpy(project->name, name, name_len);

    return MC_OK;
}

/**
 * Gets project name, or returns (unnamed) if it is unnamed.
 *
 * @return Returns project name associated with this project.
 * @internal
 */
char* mc_project_get_name(mc_project_t* project) {
    if (project->name == NULL) {
        return "(unnamed)";
    }

    return project->name;
}

/**
 * Adds mc_file_t to sqlite3 database.
 *
 * @param db sqlite3 database to operate on.
 * @param file mc_file_t to add to the database.
 * @return mc_error_t indicating success or failure.
 * @internal
 */
mc_error_t db_add_file(sqlite3* db, mc_file_t* file) {
    int rc = 0;
    sqlite3_stmt* stmt = NULL;
    const char* query = "insert into files values (?, ?);";

    /* Prepare query */
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
        return MC_ERR;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, file->name, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, file->path, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
        return MC_ERR;
    }

    /* */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
        return MC_ERR;
    }

    /* */
    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        MC_ERROR("SQL ERROR %i: %s\n", rc, sqlite3_errmsg(db));
        return MC_ERR;
    }

    return MC_OK;
}

/**
 * Adds a file to the project.
 *
 * @param project mc_project_t to add file to.
 * @param file mc_file_t to add to project.
 * @return mc_error_t indicating success or failure.
 */
mc_error_t mc_project_add_file(mc_project_t* project, mc_file_t* file) {
    mc_error_t rc = MC_ERR;
    uint32_t cur_index = 0;
    uint32_t i = 0;

    /* Ensure that file isn't null */
    if (file == NULL) {
        MC_ERROR("Input file is null.\n");
        return rc;
    }

    /* Ensure we haven't already added the file */
    for (i=0; i < project->file_amt; i++) {
        mc_file_t* curfile = project->files[i];
        if (curfile == file ||
            strcmp(curfile->name, file->name) == 0 ||
            strcmp(curfile->path, file->path) == 0) {
            MC_ERROR("Input file already has been added to project.\n");
            return rc;
        }
    }

    /* Save index that the file should be saved to, then iterate total */
    cur_index = project->file_amt;
    project->file_amt++;

    /* Create more space in the file array */
    project->files = realloc(project->files, sizeof(*project->files) * project->file_amt);
    if (project->files == NULL) {
        MC_ERROR("Failed to (re)alloc file array.\n");
        return rc;
    }

    /* Add file to the array */
    project->files[cur_index] = file;

    /* */
    rc = db_add_file(project->db, file);
    if (rc != MC_OK) {
        MC_ERROR("Failed to add file to database.\n");
        return rc;
    }

    /* Focus ctx */
    project->file_focused = file;

    return MC_OK;
}

/**
 * Removes project from file.
 *
 * @param project mc_project_t to remove file from.
 * @param file_index index of file to remove.
 * @return mc_error_t indicating success or failure.
 */
mc_error_t mc_project_remove_file(mc_project_t* project, uint32_t file_index) {

    /* Ensure that the index is valid */
    if (file_index >= project->file_amt) {
        return MC_ERR;
    }

    /* If the file to be removed is focused, set focused to NULL */
    if (project->files[file_index] == project->file_focused) {
        project->file_focused = NULL;
    }

    /* If this operation will remove the last file, just delete the array */
    if (project->file_amt == 1) {
        free(project->files);
        project->files = NULL;
        project->file_amt = 0;
        return MC_OK;
    }

    /* Shift contents of memory over to replace removed element */
    memmove(&project->files[file_index], &project->files[file_index+1],
        (project->file_amt-file_index) * sizeof(project->files[0]));

    /* Decrement the amount of files we're tracking. */
    project->file_amt--;

    /* Create more space in the file array */
    project->files = realloc(project->files, project->file_amt);
    if (project->files == NULL) {
        return MC_ERR;
    }

    return MC_OK;
}

/**
 * Retrieves file from project associated with the given file_index.
 *
 * @param project mc_project_t to get file from.
 * @param file_index index of file in project.
 * @return Pointer to mc_file_t that cooresponds with file_index (or NULL).
 */
mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index) {

    if (project->files == NULL ||
        file_index >= project->file_amt) {
        return NULL;
    }

    return project->files[file_index];
}

/**
 * Returns amount of files in project.
 *
 * @param project mc_project_t to get file amount from.
 * @return Amount of files in project.
 */
uint32_t mc_project_get_file_amount(mc_project_t* project) {
    return project->file_amt;
}

/**
 * Focuses a file.
 *
 * @param project mc_project_t to set focused file in.
 * @param file mc_file_t to set as focused.  Must be added to the project.
 * @return mc_error_t indicating success or failure.
 */
mc_error_t mc_project_set_focused_file(mc_project_t* project, mc_file_t* file) {
    uint32_t i = 0;

    /* Ensure we added the file */
    for (i=0; i < project->file_amt; i++) {
        if (file == project->files[i]) {
            /* We've found the file, focus it and return */
            project->file_focused = file;
            return MC_OK;
        }
    }

    /* We didn't find the file */
    return MC_ERR;
}

/**
 * Gets the currently focused file.
 *
 * @param project mc_project_t to get focused file from.
 * @return Pointer to mc_file_t that is focused.
 */
mc_file_t* mc_project_get_focused_file(mc_project_t* project) {
    return project->file_focused;
}
