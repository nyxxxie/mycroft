#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "file.h"

#define MDB_TABLE_PROJECT_INFO "project_info"
#define MDB_TABLE_PROJECT_CONF "project_conf"

/** Query used by mdb_kv_put_* functions to insert a kv pair */
static char* kv_put_query =
    "INSERT OR REPLACE INTO ? (key, value)"
    "VALUES (?, ?)";

static char* kv_get_query =
    "SELECT value"
    "FROM ?"
    "WHERE key == ?";

int stmt_cleanup(mc_mdb_t* mdb, sqlite3_stmt* stmt) {

    int rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "stmt_cleanup.sqlite3_finalize: %s\n",
            __func__, sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int stmt_put_init(mc_mdb_t* mdb, sqlite3_stmt** stmt) {

    int rc = sqlite3_prepare_v2(mdb->db, kv_put_query, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "stmt_get_init.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int stmt_put_run(mc_mdb_t* mdb, sqlite3_stmt* stmt) {

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "stmt_get_run.sqlite3_step: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}

int stmt_get_perform(mc_mdb_t* mdb, sqlite3_stmt** stmt, const char* table, const char* key) {

    int rc = 0;

    /* Create SQL prepared statement */
    rc = sqlite3_prepare_v2(mdb->db, kv_get_query, -1, stmt, NULL);
    if (rc != SQLITE_OK) {

        fprintf(stderr, "stmt_get_perform.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(*stmt, 1, table, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(*stmt, 2, key, -1, SQLITE_TRANSIENT) != SQLITE_OK) {

        fprintf(stderr, "stmt_get_perform.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL operation */
    rc = sqlite3_step(*stmt);
    if (rc != SQLITE_DONE) {
        if (rc == SQLITE_ROW) {
            fprintf(stderr, "stmt_get_perform.sqlite_step: Somehow query returned"
                "multiple rows.  Report this.");
        }
        else {
            fprintf(stderr, "stmt_get_perform.sqlite3_step: %s\n",
                sqlite3_errmsg(mdb->db));
        }

        return -1;
    }

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
        "value  BLOB)",
    };

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

int mdb_set_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {

    char* tmp = NULL;

    /* Set the file's name and size in the db. */
    if (mdb_set_file_name(mdb, file_name(ctx->file))) {
        return -1;
    }
    if (mdb_set_file_path(mdb, file_path(ctx->file))) {
        return -1;
    }
    if (mdb_set_file_size(mdb, file_size(ctx->file))) {
        return -1;
    }

    /* Hash the file and set the property in the table */
    SHA256_CTX shactx;
    SHA256_Init(&shactx);

    char readbuf[1024];
    while (1) {
        int amnt = file_read(ctx->file, sizeof(readbuf), readbuf);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return -1;
        }

        SHA256_Update(&shactx, readbuf, amnt);
    }

    mdb_hash_t hash;
    SHA256_Final(hash.bytes, &shactx);

    if (mdb_set_file_hash(mdb, &hash)) {
        return -1;
    }

    return 0;
}

int mdb_load_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {

}

int mdb_set_file_size(mc_mdb_t* mdb, int size) {
    return mdb_kv_put_int(mdb, MDB_TABLE_PROJECT_INFO, "file_size", size);
}

int mdb_get_file_size(mc_mdb_t* mdb, int* size) {
    return mdb_kv_get_int(mdb, MDB_TABLE_PROJECT_INFO, "file_size", size);
}

int mdb_set_file_name(mc_mdb_t* mdb, const char* name) {
    return mdb_kv_put_str(mdb, MDB_TABLE_PROJECT_INFO, "file_name", name);
}

int mdb_get_file_name(mc_mdb_t* mdb, membuf_t* name) {
    return mdb_kv_get_str(mdb, MDB_TABLE_PROJECT_INFO, "file_name", name);
}

int mdb_set_file_path(mc_mdb_t* mdb, const char* path) {
    return mdb_kv_put_str(mdb, MDB_TABLE_PROJECT_INFO, "file_path", path);
}

int mdb_get_file_path(mc_mdb_t* mdb, membuf_t* path) {
    return mdb_kv_get_str(mdb, MDB_TABLE_PROJECT_INFO, "file_path", path);
}

int mdb_set_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash) {
    return mdb_kv_put_raw(mdb, MDB_TABLE_PROJECT_INFO, "file_hash", hash->bytes, sizeof(hash));
}

int mdb_get_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Perform query */
    rc = stmt_get_perform(mdb, &stmt, MDB_TABLE_PROJECT_INFO, "file_hash");
    if (rc < 0) {
        return -1;
    }

    /* Copy column text to hash */
    void* bytes = sqlite3_column_blob(stmt, 1);  // sqlite frees these bytes later, so we need to copy them
    memcpy(hash->bytes, bytes, sizeof(hash));

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int mdb_validate(mc_mdb_t* mdb) {
    return -1;  // TODO: implement
}

int mdb_kv_put_int(mc_mdb_t* mdb, const char* table, const char* key, int val) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Initialize the sql statement */
    rc = stmt_put_init(mdb, &stmt);
    if (rc < 0) {
        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, val) != SQLITE_OK) {

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
    rc = stmt_put_init(mdb, &stmt);
    if (rc < 0) {
        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, val, strlen(val), SQLITE_TRANSIENT) != SQLITE_OK) {

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
    rc = stmt_put_init(mdb, &stmt);
    if (rc < 0) {
        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob(stmt, 3, val, size, SQLITE_TRANSIENT) != SQLITE_OK) {

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

int mdb_kv_get_int(mc_mdb_t* mdb, const char* table, const char* key, int* val) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Perform query */
    rc = stmt_get_perform(mdb, &stmt, table, key);
    if (rc < 0) {
        return -1;
    }

    /* Get desired SQL value */
    *val = sqlite3_column_int(stmt, 1);

    /* Clean up */
    rc = stmt_cleanup(mdb, stmt);
    if (rc < 0) {
        return -1;
    }

    return 0;
}

int mdb_kv_get_str(mc_mdb_t* mdb, const char* table, const char* key, membuf_t* buf) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Perform query */
    rc = stmt_get_perform(mdb, &stmt, table, key);
    if (rc < 0) {
        return -1;
    }

    /* Init membuf */
    rc = membuf_init(buf, MEMBUF_HEAP);
    if (rc < 0) {
        return -1;
    }

    /* Copy column text to membuf */
    rc = membuf_copybytes(buf,
        sqlite3_column_text(stmt, 1),
        sqlite3_column_bytes(stmt, 1) + 1);
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

int mdb_kv_get_raw(mc_mdb_t* mdb, const char* table, const char* key, membuf_t* buf) {

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Perform query */
    rc = stmt_get_perform(mdb, &stmt, table, key);
    if (rc < 0) {
        return -1;
    }

    /* Init membuf */
    rc = membuf_init(buf, MEMBUF_HEAP);
    if (rc < 0) {
        return -1;
    }

    /* Copy column text to membuf */
    rc = membuf_copybytes(buf,
        sqlite3_column_blob(stmt, 1),
        sqlite3_column_bytes(stmt, 1));
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
