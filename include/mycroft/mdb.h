#ifndef MYCROFT_MDB_H
#define MYCROFT_MDB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mycroft.h>
#include <mycroft/membuf.h>

typedef struct mc_mdb_t mc_mdb_t;

#define MDB_HASH_SIZE 20 // change this to coorespond with hash size
typedef struct {
    unsigned char bytes[MDB_HASH_SIZE];
} mdb_hash_t;

/* Init/free functions */
int mdb_init(mc_mdb_t* mdb);
int mdb_close(mc_mdb_t* mdb);

int mdb_create_db(mc_mdb_t* mdb, const char* filename);
int mdb_load_db(mc_mdb_t* mdb, const char* filename);

/* Field query */
int mdb_set_file_size(mc_mdb_t* mdb, int size);
int mdb_get_file_size(mc_mdb_t* mdb, int* size);

int mdb_set_file_name(mc_mdb_t* mdb, const char* name);
int mdb_get_file_name(mc_mdb_t* mdb, membuf_t* name);

int mdb_set_file_path(mc_mdb_t* mdb, const char* path);
int mdb_get_file_path(mc_mdb_t* mdb, membuf_t* path);

int mdb_set_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash);
int mdb_get_file_hash(mc_mdb_t* mdb, mdb_hash_t* hash);

/* Util functions */
int mdb_validate(mc_mdb_t* mdb);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_MDB_H
