#ifndef MYCROFT_CORE_H
#define MYCROFT_CORE_H

#include <mycroft/mycroft.h>
#include "file.h"
#include "config.h"
#include "database.h"

typedef struct mc_ctx_t {
    mc_config_t config;
    mc_mdb_t    db;
    mc_file_t   file;
};

#endif // MYCROFT_CORE_H
