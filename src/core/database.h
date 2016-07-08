#ifndef MYCROFT_DATABASE_H
#define MYCROFT_DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sqlite3.h>
#include <openssl/sha.h>
#include "core.h"
#include "membuf.h"

struct mc_mdb_t {
    char*    filename;
    sqlite3* db;
};

typedef struct {
    unsigned char bytes[SHA_DIGEST_LENGTH];
} mdb_hash_t;

/* Init/free functions */
int mdb_init(mc_mdb_t* mdb);
int mdb_close(mc_mdb_t* mdb);

int mdb_create_db(mc_mdb_t* mdb, const char* filename);
int mdb_load_db(mc_mdb_t* mdb, const char* filename);

//TODO: move these to core.  They don't really belong here.
int mdb_set_project(mc_mdb_t* mdb, mc_ctx_t* ctx);
int mdb_load_project(mc_mdb_t* mdb, mc_ctx_t* ctx);

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

/* Raw query functions */
int mdb_kv_put_int(mc_mdb_t* mdb, const char* table, const char* key, int val);
int mdb_kv_put_str(mc_mdb_t* mdb, const char* table, const char* key, const char* val);
int mdb_kv_put_raw(mc_mdb_t* mdb, const char* table, const char* key, void* val, int size);

int mdb_kv_get_int(mc_mdb_t* mdb, const char* table, const char* key, int* val);
int mdb_kv_get_str(mc_mdb_t* mdb, const char* table, const char* key, membuf_t* buf);
int mdb_kv_get_raw(mc_mdb_t* mdb, const char* table, const char* key, membuf_t* buf);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_DATABASE_H
