#include <stdio.h>
#include "database.h"

int db_load_from_file(mycroft_database_t* mdb, const char* db_file) {

    /* Open database file */
    int rc = sqlite3_open(db_file, &mdb->db);
    if (rc) {
        fprintf(stderr, "Couldn't open mdb file \"%s\": %s\n",
            db_file, sqlite3_errmsg(mdb->db));

        return -1;
    }

    printf("Opened database successfully\n");

    return 0;
}

int db_create_file(mycroft_database_t* mdb, const char* db_file) {

    /* Open database file */
    int rc = sqlite3_open(db_file, &mdb->db);
    if (rc) {
        fprintf(stderr, "Couldn't open mdb file \"%s\": %s\n",
            db_file, sqlite3_errmsg(mdb->db));

        return -1;
    }

    //TODO: Do this with prepared statements sqlite3_step and sqlite3_column_*

    /* Create SQL statement */
    const char* sql =                      \
        "CREATE TABLE COMPANY("            \
        "ID INT PRIMARY KEY     NOT NULL," \
        "NAME           TEXT    NOT NULL," \
        "AGE            INT     NOT NULL," \
        "ADDRESS        CHAR(50),"         \
        "SALARY         REAL );";

    /* Execute SQL statement */
    char* err_msg = 0;
    rc = sqlite3_exec(mdb->db, sql, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }

    printf("Opened database successfully\n");

    return 0;
}

int db_close(mycroft_database_t* mdb) {
    sqlite3_close(mdb->db);
    return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {

    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int db_set_target_file(mycroft_database_t* mdb, mycroft_file_t* file) {
}
