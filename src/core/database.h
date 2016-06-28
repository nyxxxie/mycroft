#ifndef MYCROFT_DATABASE_H
#define MYCROFT_DATABASE_H

#include <sqlite3.h>
#include "file.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sqlite3* db;
} mc_mdb_t;

/* Init/free functions */
int mdb_init(mc_mdb_t* mdb);
int mdb_close(mc_mdb_t* mdb);
int mdb_load_target(mc_mdb_t* mdb, mc_file_t* target_file);
//int mdb_load_mdb(mc_mdb_t* mdb, const char* mdb_file);

/* Raw db functions */
sqlite3* db_get_backend(mc_mdb_t* mdb);

/* Data query functions */

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_DATABASE_H
