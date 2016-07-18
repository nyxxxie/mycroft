#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mycroft/mdb.h>
#include <mycroft/membuf.h>
#include "database.h"
#include "file.h"

#define MDB_TABLE_PROJECT_INFO "project_info"
#define MDB_TABLE_PROJECT_CONF "project_conf"

int stmt_cleanup(mc_mdb_t* mdb, sqlite3_stmt* stmt) {

    int rc = 0;

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "stmt_cleanup.sqlite3_finalize: %s\n",
            __func__, sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int stmt_put_init(mc_mdb_t* mdb, sqlite3_stmt** stmt, const char* table) {

    int rc = 0;
    int total = 0;
    char* query = NULL;
    const char* qparts[] = {
        "INSERT OR REPLACE INTO ",
        " (key, value) VALUES (?, ?)"
    };

    /* Build up the query */
    total = strlen(table) + strlen(qparts[0]) + strlen(qparts[1]);
    query = calloc(1, total + 1);
    if (query == NULL) {
        fprintf(stderr, "stmt_put_init.calloc: Returned NULL.\n");
        return -1;
    }

    strcat(query, qparts[0]);
    strcat(query, table);
    strcat(query, qparts[1]);

    /* Prepare query */
    rc = sqlite3_prepare_v2(mdb->db, query, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "stmt_put_init.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int stmt_put_run(mc_mdb_t* mdb, sqlite3_stmt* stmt) {

    int rc = 0;

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "stmt_put_run.sqlite3_step: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int mdb_kv_put_int(mc_mdb_t* mdb, const char* table, const char* key, int val) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Initialize the sql statement */
    rc = stmt_put_init(mdb, &stmt, table);
    if (rc < 0) {
        return -1;
    }

    // TODO: refactor these functions to just take a sqlite_value or w/e.
    //       then we can just pass that and make the rest of this generic.

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, val) != SQLITE_OK) {

        fprintf(stderr, "mdb_kv_put_int.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL operation */
    rc = stmt_put_run(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int mdb_kv_put_str(mc_mdb_t* mdb, const char* table, const char* key, const char* val) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Initialize the sql statement */
    rc = stmt_put_init(mdb, &stmt, table);
    if (rc < 0) {
        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, val, strlen(val), SQLITE_TRANSIENT) != SQLITE_OK) {

        fprintf(stderr, "mdb_kv_put_str.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL operation */
    rc = stmt_put_run(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int mdb_kv_put_raw(mc_mdb_t* mdb, const char* table, const char* key, void* val, int size) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Initialize the sql statement */
    rc = stmt_put_init(mdb, &stmt, table);
    if (rc < 0) {
        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob(stmt, 2, val, size, SQLITE_TRANSIENT) != SQLITE_OK) {

        fprintf(stderr, "mdb_kv_put_raw.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL operation */
    rc = stmt_put_run(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
int stmt_get_perform(mc_mdb_t* mdb, sqlite3_stmt** stmt, const char* table, const char* key) {

    int rc = 0;
    int total = 0;
    char* query = NULL;
    const char* qparts[] = {
        "SELECT value FROM ",
        " WHERE key = ?"
    };

    /* Build up the query */
    total = strlen(table) + strlen(qparts[0]) + strlen(qparts[1]);
    query = calloc(1, total + 1);
    if (query == NULL) {
        fprintf(stderr, "stmt_get_perform.calloc: Returned NULL.\n");
        return -1;
    }

    strcat(query, qparts[0]);
    strcat(query, table);
    strcat(query, qparts[1]);

    /* Create SQL prepared statement */
    rc = sqlite3_prepare_v2(mdb->db, query, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "stmt_get_perform.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(*stmt, 1, key, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        fprintf(stderr, "stmt_get_perform.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL operation */
    rc = sqlite3_step(*stmt);
    //if (rc != SQLITE_DONE) {
    //    if (rc == SQLITE_ROW) {
    //        fprintf(stderr, "stmt_get_perform.sqlite_step: Somehow query"
    //           " returned multiple rows.  Report this.\n");
    //    }
    //    else {
    //        fprintf(stderr, "stmt_get_perform.sqlite3_step: %s\n",
    //            sqlite3_errmsg(mdb->db));
    //    }

    //    return -1;
    //}

    return 0;
}

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
    mdb->filename = NULL;
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

    if (mdb->filename != NULL)
        free(mdb->filename);

    return 0;
}

/**
 * Creates the default tables for a mdb.
 *
 * @param mdb Mycroft db context to operate on.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_create_default_tables(mc_mdb_t* mdb) {

    const char* create_queries[] = {
        "CREATE TABLE project_info("
        "key    TEXT    UNIQUE,"
        "value  BLOB)",
        "CREATE TABLE project_conf("
        "key    TEXT    UNIQUE,"
        "value  BLOB)"
    };

    // TODO: make function that creates a key/value table (so we can make more
    //       easily for testing

    for (int i = 0; i < 2; i++) {

        sqlite3_stmt* stmt;
        int rc = 0;

        if ((rc = sqlite3_prepare_v2(mdb->db,
            create_queries[i], -1,
            &stmt, NULL)) != SQLITE_OK) {

            fprintf(stderr, "mdb_create_default_table.sqlite3_prepare_v2: %s\n",
                sqlite3_errmsg(mdb->db));

            return -1;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "mdb_create_default_table.sqlite3_step: %s\n",
                sqlite3_errmsg(mdb->db));

            return -1;
        }

        rc = sqlite3_finalize(stmt);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "mdb_create_default_table.sqlite3_finalize: %s\n",
                sqlite3_errmsg(mdb->db));

            return -1;
        }
    }

    return 0;
}

/**
 * Creates a new mdb file.  Will fail if the db file exists.
 *
 * @param mdb Mycroft db context to operate on.
 * @param filename Mycroft db file to use.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_create_db(mc_mdb_t* mdb, const char* filename) {

    /* Ensure that the file doesn't already exist */
    if (file_exists(filename) == 0) {
        return -1;
    }

    /* Create sqlite db */
    int rc = sqlite3_open_v2(
        filename, &mdb->db,
        SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_create_db.sqlite3_open_v2(\"%s\"): %s\n",
            filename, sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Create the default tables */
    rc = mdb_create_default_tables(mdb);
    if (rc < 0) {
        return rc;
    }

    return 0;
}

/**
 * Opens an existing mdb file.  Will fail if the db file doesn't exist.
 *
 * @param mdb Mycroft db context to operate on.
 * @param filename Mycroft db file to use.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_load_db(mc_mdb_t* mdb, const char* filename) {

    /* Create sqlite db */
    int rc = sqlite3_open_v2(
        filename, &mdb->db,
        SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_load_db.sqlite3_open_v2(\"%s\"): %s\n",
            filename, sqlite3_errmsg(mdb->db));
        return -1;
    }

    return 0;
}

int mdb_kv_get_value(mc_mdb_t* mdb, const char* table, const char* key, sqlite3_value** val) {

    int rc = 0;
    sqlite3_stmt* stmt;
    sqlite3_value* tmp;

    /* Perform query */
    rc = stmt_get_perform(mdb, &stmt, table, key);
    if (rc < 0) {
        return -1;
    }

    /* Get desired SQL value and duplicate it for thread safety */
    tmp = sqlite3_column_value(stmt, 0);
    *val = sqlite3_value_dup(tmp);
    if (*val == NULL) {
        //TODO: print reason
        return -1;
    }

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int mdb_set_file_size(mc_mdb_t* mdb, int size) {
    return mdb_kv_put_int(mdb, MDB_TABLE_PROJECT_INFO, "file_size", size);
}

int mdb_get_file_size(mc_mdb_t* mdb, int* size) {

    int rc = 0;
    sqlite3_value* val;

    /* Perform query */
    rc = mdb_kv_get_value(mdb, MDB_TABLE_PROJECT_INFO, "file_size", &val);
    if (rc < 0) {
        return -1;
    }

    /* Get value */
    *size = sqlite3_value_int(val);

    /* Clean up and return */
    sqlite3_value_free(val);
    return 0;
}

int mdb_set_file_name(mc_mdb_t* mdb, const char* name) {
    return mdb_kv_put_str(mdb, MDB_TABLE_PROJECT_INFO, "file_name", name);
}

int mdb_get_file_name(mc_mdb_t* mdb, membuf_t* name) {

    int rc = 0;
    sqlite3_value* val;

    /* Perform query */
    rc = mdb_kv_get_value(mdb, MDB_TABLE_PROJECT_INFO, "file_name", &val);
    if (rc < 0) {
        return -1;
    }

    /* Get value */
    rc = membuf_copybytes(name,
        sqlite3_value_text(val),
        sqlite3_value_bytes(val) + 1);
    if (rc < 0) {
        return -1;
    }

    /* Clean up and return */
    sqlite3_value_free(val);
    return 0;
}

int mdb_set_file_path(mc_mdb_t* mdb, const char* path) {
    return mdb_kv_put_str(mdb, MDB_TABLE_PROJECT_INFO, "file_path", path);
}

int mdb_get_file_path(mc_mdb_t* mdb, membuf_t* path) {

    int rc = 0;
    sqlite3_value* val;

    /* Perform query */
    rc = mdb_kv_get_value(mdb, MDB_TABLE_PROJECT_INFO, "file_path", &val);
    if (rc < 0) {
        return -1;
    }

    /* Get value */
    rc = membuf_copybytes(path,
        sqlite3_value_text(val),
        sqlite3_value_bytes(val) + 1);
    if (rc < 0) {
        return -1;
    }

    /* Clean up and return */
    sqlite3_value_free(val);
    return 0;
}

int mdb_set_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash) {
    return mdb_kv_put_raw(mdb, MDB_TABLE_PROJECT_INFO, "file_hash", hash->bytes, MDB_HASH_SIZE);
}

int mdb_get_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash) {

    int rc = 0;
    sqlite3_value* val;

    /* Perform query */
    rc = mdb_kv_get_value(mdb, MDB_TABLE_PROJECT_INFO, "file_hash", &val);
    if (rc < 0) {
        return -1;
    }

    /* Get value */
    void* bytes = sqlite3_value_blob(val);
    memcpy(hash->bytes, bytes, MDB_HASH_SIZE);

    /* Clean up and return */
    sqlite3_value_free(val);
    return 0;
}

int mdb_validate(mc_mdb_t* mdb) {
    return -1;  // TODO: implement
}
