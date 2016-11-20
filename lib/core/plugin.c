#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <Python.h>
#include <mycroft/plugin.h>
#include "binds/file.h"
#include "binds/context.h"
#include "file.h"
#include "plugin.h"
#include "config.h"

/**
 * @brief Initialize an mc_plugin_entry_t
 * @param entry Entry to free
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int mc_plugin_entry_init(mc_plugin_entry_t** entry) {
    mc_plugin_entry_t* tmp = NULL;

    /* Allocate space for the entry */
    tmp = (mc_plugin_entry_t*)malloc(sizeof(mc_plugin_entry_t));
    if (tmp == NULL) {
        return -1;
    }

    /* Set all values in entry to their default values */
    tmp->name      = 0;
    tmp->version   = 0;
    tmp->module    = NULL;
    tmp->entryfunc = NULL;
    tmp->next      = NULL;
    tmp->prev      = NULL;

    *entry = tmp;

    return 0;
}


/**
 * @brief Free an mc_plugin_entry_t
 * @param entry Entry to free
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int mc_plugin_entry_free(mc_plugin_entry_t* entry) {
    mc_plugin_entry_t* next = NULL;
    mc_plugin_entry_t* prev = NULL;

    if (entry->name != NULL)
        free(entry->name);

    if (entry->version != NULL)
        free(entry->version);

    if (entry->module != NULL)
        Py_DECREF(entry->module);

    if (entry->next != NULL && entry->prev != NULL) {
        if (entry == plugin_first) {
            next = entry->next;
            prev = entry->prev;
            prev->next = next;
            next->prev = prev;
        }
        else if (entry == plugin_last) {
            plugin_last = entry->prev;
            plugin_last->next = NULL;
        }
        else {
            plugin_first = entry->next;
            plugin_first->prev = NULL;
        }
    }

    free(entry);

    return 0;
}

/**
 * @brief Adds all the library bindings to the python inittab.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int init_bindings() {
    int rc = 0;

    rc = init_binds_file();
    if (rc < 0) {
        return rc;
    }

    //rc = init_binds_core();
    //if (rc < 0) {
    //    return rc;
    //}

    return rc;
}

/**
 * @brief Adds various locations to the python path for plugin purposes.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int init_path(mc_ctx_t* ctx) {
    int rc = 0;
    int i = 0;

    const char* default_path[] = {
        MYCROFT_LOCAL_PLUGINS,
        MYCROFT_LOCAL_MODULES,
        MYCROFT_PLUGINS,
        MYCROFT_MODULES
    };

    for (i=0; i < 4; i++) {
        rc = mc_plugin_addpath(ctx, default_path[i]);
        if (rc < 0) {
            return rc;
        }
    }

    return 0;
}

int load_plugin(mc_ctx_t* ctx, const char* plugins_dir, const char* plugin_dir) {
    int rc = 0;
    char* initfile = NULL;

    /* Alloc space for plugin_dir path with PLUGIN_INIT_FILE appended to it */
    initfile = (char*)alloca(strlen(plugins_dir)
        + strlen(plugin_dir)
        + strlen(PLUGIN_INIT_FILE)
        + 2);

    /* Apend PLUGIN_INIT_FILE to plugin_dir */
    strcpy(initfile, plugins_dir);
    strcat(initfile, "/");
    strcat(initfile, plugin_dir);
    strcat(initfile, "/" PLUGIN_INIT_FILE);

    /* Check if the init file exists for this plugin */
    rc = file_exists(initfile);
    if (rc < 0) {
        fprintf(stderr, "Failed to load plugin at \"%s\": couldn't find init file.\n",
            plugin_dir);
        printf("%s\n", initfile);
        return rc;
    }

    /* Load the plugin */
    rc = mc_plugin_load(ctx, plugin_dir);
    if (rc < 0) {
        return rc;
    }

    return 0;
}

int load_plugin_dir(mc_ctx_t* ctx, const char* plugins_dir) {
    DIR* dir;
    struct dirent* dent;

    dir = opendir(plugins_dir);

    if (dir != NULL) {
        while ((dent = readdir(dir)) != NULL) {
            if (dent->d_type == DT_DIR) {
                int i;
                int good=1;

                /* Look for . char */
                for (i=0; i < strlen(dent->d_name); i++) {
                    if (dent->d_name[i] == '.') {
                        good=0;
                        break;
                    }
                }

                if (good != 0) {
                    load_plugin(ctx, plugins_dir, dent->d_name);
                }
            }
        }

        closedir(dir);
    }

    return 0;
}

/**
 * Loads all plugins from the plugin directory(s).  Tries to account for
 * dependencies.
 *
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int mc_plugin_load_plugins(mc_ctx_t* ctx) {
    int i = 0;
    int rc = 0;

    const char* plugin_path[] = {
        MYCROFT_LOCAL_PLUGINS,
        MYCROFT_PLUGINS
    };

    for (i=0; i < 2; i++) {
        rc = load_plugin_dir(ctx, plugin_path[i]);
        if (rc < 0) {
            return rc;
        }
    }

    return 0;
}

/**
 * Unloads all of the plugins that are loaded.
 *
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int unload_plugins() {
    int rc = 0;
    mc_plugin_entry_t* entry = NULL;

    /* Loop through all entries */
    entry = plugin_first;
    while (entry != NULL) {
        mc_plugin_entry_t* next = entry->next;
        rc = mc_plugin_entry_free(entry);
        if (rc < 0) {
            return rc;
        }

        /* Proceed to next entry */
        entry = next;
    }

    return 0;
}

/**
 * Initialize plugin system.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_init(mc_ctx_t* ctx) {
    int rc = 0;

    /* */
    rc = init_bindings(ctx);
    if (rc < 0) {
        return rc;
    }

    Py_Initialize();

    /* */
    rc = init_path(ctx);
    if (rc < 0) {
        return rc;
    }

    /* */
    plugin_first = NULL;
    plugin_last  = NULL;

    /* */
    rc = mc_plugin_load_plugins(ctx);
    if (rc < 0) {
        return rc;
    }

    return 0;
}

/**
 * Close plugin system.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_close(mc_ctx_t* ctx) {
    int rc = 0;

    rc = unload_plugins(ctx);
    if (rc < 0) {
        return rc;
    }

    Py_Finalize();

    return 0;
}

/**
 * Adds a directory for the plugin system to scan for plugins.
 *
 * @param path Path to add.
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_addpath(mc_ctx_t* ctx, const char* path) {
    PyObject *pypath=NULL, *localname=NULL;

    pypath = PySys_GetObject("path");
    localname = PyUnicode_FromString(path);

    PyList_Append(pypath, localname);

    Py_DECREF(localname);

    return 0;
}

/**
 *
 */
int mc_plugin_runscript(mc_ctx_t* ctx, const char* script) {
    int rc = 0;

    /* Run script */
    rc = PyRun_SimpleString(script);
    if (rc < 0) {
        return rc;
    }

    return 0;
}

/**
 *
 */
int mc_plugin_runfile(mc_ctx_t* ctx, const char* script) {
    int rc = 0;
    FILE* file = NULL;;

    /* Open file */
    file = fopen(script, "r");
    if (file == NULL) {
        fprintf(stdout, "Failed to open file \"%s\": %s\n", script, strerror(errno));
        return -1;
    }

    /* Run script */
    rc = PyRun_SimpleFileEx(file, script, 1);
    if (rc < 0) {
        PyErr_Print();
        fprintf(stdout, "Failed to run python script \"%s\".\n", script);
        return rc;
    }

    return 0;
}

/**
 * Utility function meant to be used to call the various plugin info functions
 * located in the plugin init file.
 *
 * @param module Python module object to operate on.
 * @param funcname Function to call.
 * @return Returns the return object of the specified func (or NULL!)
 * @internal
 */
PyObject* call_plugin_infofunc(PyObject* module, const char* funcname) {
    PyObject* func=NULL, *value=NULL;

    /* Find the function */
    func = PyObject_GetAttrString(module, funcname);
    if (func == NULL || PyCallable_Check(func) == 0) {
        printf("Couldn't find infofunc \"%s\".\n", funcname);
        return NULL;
    }

    value = PyObject_CallObject(func, NULL);
    Py_DECREF(func);
    if (value == NULL) {
        PyErr_Print();
        printf("Couldn't call infofunc \"%s\".\n", funcname);
        return NULL;
    }

    return value;
}

/**
 *
 * @param module Python module to use.
 * @param entry mc_plugin_entry_t to operate on.
 * @param path Directory of plugin.  Used for debugging messages.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_name(PyObject* module, mc_plugin_entry_t* entry, const char* path) {
    PyObject* value=NULL;
    char* str=NULL;

    /* Call plugin name function */
    value = call_plugin_infofunc(module, PLUGIN_NAME_FUNC);
    if (value == NULL) {
        return -1;
    }

    /* Get the string value of the return value and copy it to entry */
    str = PyUnicode_AsUTF8(value);
    entry->name = (char*)malloc(strlen(str));
    if (entry->name == NULL) {
        printf("PLUGIN \"%s\": Failed to allocate space for name str.\n",
            path);
        return -1;
    }
    strcpy(entry->name, str);

    Py_DECREF(value);
    return 0;
}

/**
 *
 * @param module Python module to use.
 * @param entry mc_plugin_entry_t to operate on.
 * @param path Directory of plugin.  Used for debugging messages.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_version(PyObject* module, mc_plugin_entry_t* entry, const char* path) {
    PyObject* value=NULL;
    char* str=NULL;

    /* Call plugin name function */
    value = call_plugin_infofunc(module, PLUGIN_VER_FUNC);
    if (value == NULL) {
        return -1;
    }

    /* Get the string value of the return value and copy it to entry */
    str = PyUnicode_AsUTF8(value);
    entry->version = (char*)malloc(strlen(str));
    if (entry->version == NULL) {
        printf("PLUGIN \"%s\": Failed to allocate space for version str.\n",
            path);
        return -1;
    }
    strcpy(entry->version, str);

    Py_DECREF(value);
    return 0;
}

/**
 *
 * @param module Python module to use.
 * @param entry mc_plugin_entry_t to operate on.
 * @param path Directory of plugin.  Used for debugging messages.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_depends(PyObject* module, mc_plugin_entry_t* entry, const char* path) {
    PyObject* value=NULL;
    char* str=NULL;

    /* Call plugin name function */
    value = call_plugin_infofunc(module, PLUGIN_DEPS_FUNC);
    if (value == NULL) {
        return -1;
    }

    //TODO: do stuff

    Py_DECREF(value);
    return 0;
}

/**
 *
 * @param module Python module to use.
 * @param entry mc_plugin_entry_t to operate on.
 * @param path Directory of plugin.  Used for debugging messages.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_entry(PyObject* module, mc_plugin_entry_t* entry, const char* path) {
    PyObject* value=NULL;
    char* str=NULL;

    /* Call plugin name function */
    value = call_plugin_infofunc(module, PLUGIN_ENTRY_FUNC);
    if (value == NULL) {
        return -1;
    }

    /* Check to see if the value returned is a function */
    if (PyCallable_Check(value) == 0) {
        Py_DECREF(value);
        printf("PLUGIN \"%s\": \"%s\" return isn't a function.\n",
            path, PLUGIN_ENTRY_FUNC);
        return -1;
    }
    entry->entryfunc = value;

    return 0;
}

/**
 * Loads a plugin.  This must point to the plugin.py file, or whatever file has
 * the various information functions.
 *
 * @param path Plugin file or directory.
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_load(mc_ctx_t* ctx, const char* path) {
    int rc = 0;
    mc_plugin_entry_t* entry = NULL;
    PyObject* name = NULL;

    /* Create a python unicode string */
    name = PyUnicode_FromString(path);
    if (name == NULL) {
        return -1;
    }

    /* Initialize the entry */
    rc = mc_plugin_entry_init(&entry);
    if (rc < 0) {
        return rc;
    }

    /* Import the plugin entry file as a module */
    entry->module = PyImport_Import(name);
    Py_DECREF(name);
    if (entry->module == NULL) {
        PyErr_Print();
        printf("Couldn't open module \"%s\".\n", path);
        return -1;
    }

    /* */
    rc = get_plugin_name(entry->module, entry, path);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_version(entry->module, entry, path);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_depends(entry->module, entry, path);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_entry(entry->module, entry, path);
    if (rc < 0) {
        return rc;
    }

    printf("FOUND PLUGIN:\n");
    printf("\tNAME: %s\n", entry->name);
    printf("\tVER:  %s\n", entry->version);

    /* Create mycroft ctx python object and place it in an argument tuple */
    PyObject* args = PyTuple_New(1);
    PyObject* mctx = NULL; // todo: replace with ctx decl
    if (PyTuple_SetItem(args, 0, mctx) != 0) {
        return -1;
    }

    /* Call plugin entry point */
    PyObject_CallObject(entry->entryfunc, args);

    /* Either start the list or add the entry to the end of the list */
    if (plugin_first == NULL) {
        plugin_first = entry;
        plugin_last  = entry;
    }
    else {
        plugin_last->next = entry;
        entry->prev = plugin_last;
        plugin_last = entry;
    }

    return 0;
}

/**
 * Unloads a plugin.
 *
 * @param path Plugin file or directory.
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_unload(mc_ctx_t* ctx, const char* name) {
    int rc = 0;
    mc_plugin_entry_t* entry=NULL, *next=NULL, *prev=NULL;

    /* Loop through all entries */
    entry = plugin_first;
    while (entry != NULL) {
        if (strcmp(name, entry->name) == 0) {

            rc = mc_plugin_entry_free(entry);
            if (rc < 0) {
                return rc;
            }

            return 0;
        }

        /* Proceed to next entry */
        entry = entry->next;
    }

    return -1;
}
