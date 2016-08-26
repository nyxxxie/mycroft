#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#include <mycroft/mycroft.h>
#include <mycroft/file.h>
#include <mycroft/mdb.h>

/* Forward declarations */
typedef struct mc_config_t mc_config_t;

/** Represents the state of a file opened in mycroft */
struct mc_ctx_t {
    mc_config_t* config;
    mc_mdb_t*    db;
    mc_file_t*   file;
};

static mc_ctx_t* mc_active_ctx = NULL;

#endif // MYCROFT_INT_CORE_H
