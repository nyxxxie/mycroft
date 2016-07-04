#ifndef MYCROFT_CORE_H
#define MYCROFT_CORE_H

#include <mycroft/mycroft.h>

/* Forward declarations */
typedef struct mc_config_t mc_config_t;
typedef struct mc_mdb_t mc_mdb_t;
typedef struct mc_file_t mc_file_t;

/** Represents the state of a file opened in mycroft */
struct mc_ctx_t {
    mc_config_t* config;
    mc_mdb_t*    db;
    mc_file_t*   file;
};

#endif // MYCROFT_CORE_H
