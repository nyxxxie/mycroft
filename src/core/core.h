#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#include <mycroft/mycroft.h>
#include <mycroft/file.h>
#include <mycroft/mdb.h>

/** Represents the state of a file opened in mycroft */
struct mc_ctx_t {
    mc_mdb_t*    db;
    mc_file_t*   file;
};

#endif // MYCROFT_INT_CORE_H
