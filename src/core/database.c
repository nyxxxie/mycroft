#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

/**
 * This function initializes a given mycroft db context.  You MUST call this
 * function on your context before you use it.  Failure to do so will result in
 * undefined behavior!
 *
 * @param mdb Mycroft db context to init.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_init(mc_mdb_t* mdb) {
    mdb->db = NULL;
    return 0;
}

/**
 * Cleans up a mycroft db context.  This will close the db and free any
 * allocated memory.  Call this when you're done using a context.
 *
 * @param mdb Mycroft db context to close.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_close(mc_mdb_t* mdb) {

    if (mdb->db != NULL)
        sqlite3_close(mdb->db);

    return 0;
}

int mdb_create_default_tables(mc_mdb_t* mdb) {
    const char* create_queries[]  = {
        "CREATE TABLE project_info("
        "key    TEXT               "
        "value  TEXT               ",
        "CREATE TABLE project_settings("
        "key    TEXT                   "
        "value  TEXT                   ",
    };

    printf("str_amnt = %i\n", sizeof(create_queries));
    for (int i = 0; i < sizeof(create_queries); i++) {
        printf("%i: %s\n", i, create_queries[i]);
    }

    return 0;
}

int mdb_set_project_info(mc_mdb_t* mdb) {

    return -1;
}

int mdb_set_config_info(mc_mdb_t* mdb) {

    return -1;
}


/**
 * Loads an mycroft db context given a target file.  Will attempt to find the
 * db on disk if it exists, otherwise it will be created.
 *
 * @param mdb Mycroft db context to close.
 * @param target_file File to use.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_load_target(mc_mdb_t* mdb, mc_file_t* target_file) {

    /* Copy file path to our db path */
    char* target_filename = file_name(target_file);
    char* mdb_filename = (char*)malloc(strlen(target_filename) + 5);
    if (mdb_filename == NULL) {
        fprintf(stderr, "Failed to allocate space for mdb_filename");
        return -1;
    }
    strncpy(mdb_filename, target_filename, strlen(target_filename));

    /* Strip the file extension if we need to */
    //if (ctx->config.db_strip_orig_ext) {
    //    //TODO: I don't think we should actually make this an option
    //    char* lastslash = strrchr(mdb_filename, '/');
    //    char* lastdot = strrchr(mdb_filename, '.');
    //    if (lastslash < lastdot) {
    //        mdb_filename[lastdot-mdb_filename] = '\0';
    //    }
    //}

    /* Append the database extension */
    int b = strlen(mdb_filename);
    mdb_filename[b]   = '.';
    mdb_filename[b+1] = 'm';
    mdb_filename[b+2] = 'd';
    mdb_filename[b+3] = 'b';
    mdb_filename[b+4] = '\0';

    /* Check to see if the mdb file exists */
    int exists = (file_exists(mdb_filename) == 0);

    /* Open/create sqlite db */
    int rc = sqlite3_open(mdb_filename, &mdb->db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Couldn't open mdb file \"%s\": %s\n",
            mdb_filename, sqlite3_errmsg(mdb->db));
        free(mdb_filename);
        return -1;
    }
    free(mdb_filename);

    /* If we're making the db, we need to add tables and project info */
    if (!exists) {
        if (mdb_create_default_tables(mdb) < 0 ||
            mdb_set_project_info(mdb) < 0 ||
            mdb_set_config_info(mdb) < 0) {

            return -1;
        }
    }

    return 0;
}
