#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "database.h"
#include "file.h"

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
    tmp = file_name(ctx->file);
    if (mdb_set_pinfo_entry(mdb, "file_name", tmp, strlen(tmp))) {
        return -1;
    }
    tmp = file_path(ctx->file);
    if (mdb_set_pinfo_entry(mdb, "file_path", tmp, strlen(tmp))) {
        return -1;
    }
    fsize_t fsize = file_size(ctx->file);
    if (mdb_set_pinfo_entry(mdb, "file_size", (uint8_t*)&fsize, sizeof(fsize_t))) {
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

    char hash[SHA_DIGEST_LENGTH];
    SHA256_Final(hash, &shactx);

    if (mdb_set_pinfo_entry(mdb, "file_hash", hash, sizeof(hash))) {
        return -1;
    }

    /* */

    return 0;
}

int mdb_load_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {

}

/**
 * Inserts a key/value pair into the project_info table in the mdb.  Updates
 * the value if it already exists.
 *
 * @param mdb Mycroft db context to operate on.
 * @param key Entry key.
 * @param value Entry value.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_set_pinfo_entry(mc_mdb_t* mdb, char* key, uint8_t* value, int size) {

    const char* insert_query =
        "REPLACE INTO project_info (key, value)"
        "VALUES (?1, ?2)";

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Prepate SQL statement */
    rc = sqlite3_prepare_v2(mdb->db, insert_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_insert_pinfo_entry.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob(stmt, 2, value, size, SQLITE_TRANSIENT) != SQLITE_OK) {

        fprintf(stderr, "mdb_insert_pinfo_entry.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "mdb_insert_pinfo_entry.sqlite3_step: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Delete SQL statement */
    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_insert_pinfo_entry.sqlite3_finalize: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;

}

/**
 * Inserts a key/value pair into the project_conf table in the mdb.  Updates
 * the value if it already exists.
 *
 * @param mdb Mycroft db context to operate on.
 * @param key Entry key.
 * @param value Entry value.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mdb_set_pconf_entry(mc_mdb_t* mdb, char* key, uint8_t* value, int size) {

    const char* insert_query =
        "REPLACE INTO project_info (key, value)"
        "VALUES (?, ?)";

    int rc = 0;
    sqlite3_stmt* stmt;

    /* Prepate SQL statement */
    rc = sqlite3_prepare_v2(mdb->db, insert_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_insert_pconf_entry.sqlite3_prepare_v2: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Bind values to SQL statement */
    if (sqlite3_bind_text(stmt, 0, key, -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_blob(stmt, 1, value, size, SQLITE_TRANSIENT) != SQLITE_OK) {

        fprintf(stderr, "mdb_insert_pconf_entry.sqlite3_bind_text: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Perform SQL statement */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "mdb_insert_pconf_entry.sqlite3_step: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    /* Delete SQL statement */
    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "mdb_insert_pconf_entry.sqlite3_finalize: %s\n",
            sqlite3_errmsg(mdb->db));

        return -1;
    }

    return 0;
}
