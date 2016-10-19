#ifndef MYCROFT_INT_PROJECT_H
#define MYCROFT_INT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mdb.h>
#include "file.h"
#include "database.h"

struct mc_project_t {
    char*       name;
    mc_file_t** files;
    uint32_t    file_amt;
    mc_file_t*  file_focused;
    mc_mdb_t*   db;
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_PROJECT_H
