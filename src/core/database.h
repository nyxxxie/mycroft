#ifndef MYCROFT_DATABASE_H
#define MYCROFT_DATABASE_H

#include <stdint.h>
#include <sqlite3.h>
#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mc_mdb_t {
    char*    filename;
    sqlite3* db;
};

/* Init/free functions */
int mdb_init(mc_mdb_t* mdb);
int mdb_close(mc_mdb_t* mdb);

int mdb_create_db(mc_mdb_t* mdb, const char* filename);
int mdb_load_db(mc_mdb_t* mdb, const char* filename);

int mdb_set_project(mc_mdb_t* mdb, mc_ctx_t* ctx);
int mdb_load_project(mc_mdb_t* mdb, mc_ctx_t* ctx);

/* Data query functions */
int mdb_set_pinfo_entry(mc_mdb_t* mdb, char* key, uint8_t* value, int size);
int mdb_set_pconf_entry(mc_mdb_t* mdb, char* key, uint8_t* value, int size);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_DATABASE_H

//TODO: implement the above functions and use them in mycroft_load_file and friends.  We should have seperate create_db and load_db functions to open and create the db, as well as functions to insert/modify stored project/file settings and load project settings.  Be sure in the latter option when we load file settings we load the file from the stored path and make sure it matches the stored hash.

//TODO: also figure out how to insert an entry if it doesn't exist, otherwise update the value.
