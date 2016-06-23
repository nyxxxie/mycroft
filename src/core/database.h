#ifndef MYCROFT_DATABASE_H
#define MYCROFT_DATABASE_H

#include <sqlite3.h>
#include "file.h"

typedef struct {
    sqlite3* db;
} mycroft_database_t;

int db_load_from_file(mycroft_database_t* mdb, const char* db_file);
int db_create_file(mycroft_database_t* mdb, const char* db_file);
int db_close(mycroft_database_t* mdb);

int db_set_target_file(mycroft_database_t* mdb, mycroft_file_t* file);

#endif // MYCROFT_DATABASE_H
