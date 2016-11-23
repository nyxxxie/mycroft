#ifndef MYCROFT_INT_PROJECT_H
#define MYCROFT_INT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sqlite3.h>
#include <mycroft/project.h>
#include "file.h"

struct mc_project_t {
    char*       name;
    mc_file_t** files;
    uint32_t    file_amt;
    mc_file_t*  file_focused;
    sqlite3* db;
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_PROJECT_H
