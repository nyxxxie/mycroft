#ifndef MYCROFT_INT_PLUGIN_H
#define MYCROFT_INT_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <mycroft/plugin.h>

/**
 * Represents a plugin that is currently loaded.
 */
struct mc_plugin_entry_t {
    char* name;
    char* version;
    PyObject* module;
    PyObject* entryfunc;
    mc_plugin_entry_t* next;
    mc_plugin_entry_t* prev;
};

static mc_plugin_entry_t* plugin_first = NULL;
static mc_plugin_entry_t* plugin_last = NULL;

int mc_plugin_entry_init(mc_plugin_entry_t** entry);
int mc_plugin_entry_free(mc_plugin_entry_t* entry);

#define PLUGIN_ENTRY_FUNC "_mc_entry"
#define PLUGIN_NAME_FUNC  "_mc_name"
#define PLUGIN_VER_FUNC   "_mc_version"
#define PLUGIN_DEPS_FUNC  "_mc_depends"

#define PLUGIN_INIT_FILE  "__init__.py"

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_PLUGIN_H