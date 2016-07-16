#ifndef MYCROFT_INT_DATABASE_H
#define MYCROFT_INT_DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sqlite3.h>
#include <openssl/sha.h>
#include <mycroft/mdb.h>
#include <mycroft/membuf.h>
#include "core.h"

struct mc_mdb_t {
    char*    filename;
    sqlite3* db;
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_DATABASE_H
