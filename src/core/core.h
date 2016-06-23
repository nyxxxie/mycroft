#ifndef MYCROFT_CORE_H
#define MYCROFT_CORE_H

#include <mycroft/mycroft.h>
#include "file.h"
#include "config.h"
#include "database.h"

typedef struct mycroft_context_t {
    mycroft_config_values_t config;
    mycroft_database_t      db;
    mycroft_file_t          file;
};

#endif // CORE_H
