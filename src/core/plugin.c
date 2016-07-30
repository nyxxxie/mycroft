#include <Python.h>
#include <mycroft/plugin.h>
#include "plugin.h"

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
    //TODO: implement as we add bindings
}

/**
 * Loads all plugins from the plugin directory(s).  Tries to account for
 * dependencies.
 *
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int load_plugins() {
    //TODO: implement this
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
int mc_plugin_init() {

    int rc = 0;

    /* */
    rc = init_bindings();
    if (rc < 0) {
        return rc;
    }

    Py_Initialize();

    /* */
    plugin_first = NULL;
    plugin_last  = NULL;

    /* */
    rc = load_plugins();
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
int mc_plugin_close() {

    int rc = 0;

    rc = unload_plugins();
    if (rc < 0) {
        return rc;
    }

    Py_Finalize();

    return -1;
}

/**
 * Adds a directory for the plugin system to scan for plugins.
 *
 * @param path Path to add.
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_addpath(const char* path) {

    PyObject *pypath=NULL, *localname=NULL;

    pypath = PySys_GetObject("path");
    localname = PyUnicode_FromString(path);
    PyList_Append(path, localname);

    Py_DECREF(localname);

    return 0;
}

/**
 *
 */
int mc_plugin_runscript(const char* script) {

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
int mc_plugin_runfile(const char* script) {

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
 *
 * @param module Python module to use.
 * @param entry mc_plugin_entry_t to operate on.
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_name(PyObject* module, mc_plugin_entry_t* entry) {

    PyObject* func=NULL, *value=NULL;
    char* str=NULL;

    func = PyObject_GetAttrString(module, PLUGIN_NAME_FUNC);
    if (func == NULL || PyCallable_Check(func) == 0) {
        PyErr_Print();
        printf("Couldn't find entry point.\n");
        return -1;
    }

    value = PyObject_CallObject(func, NULL);
    Py_DECREF(func);
    if (value == NULL) {
        PyErr_Print();
        printf("Couldn't call entry point.\n");
        return -1;
    }

    str = PyUnicode_AsUTF8(value);
    entry->name = (char*)malloc(strlen(str));
    if (entry->name == NULL) {
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
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_version(PyObject* module, mc_plugin_entry_t* entry) {

    PyObject* func=NULL, *value=NULL;
    char* str;

    func = PyObject_GetAttrString(module, PLUGIN_VER_FUNC);
    if (func == NULL || PyCallable_Check(func) == 0) {
        PyErr_Print();
        printf("Couldn't find entry point.\n");
        return -1;
    }

    value = PyObject_CallObject(func, NULL);
    Py_DECREF(func);
    if (value == NULL) {
        PyErr_Print();
        printf("Couldn't call entry point.\n");
        return -1;
    }

    str = PyUnicode_AsUTF8(value);
    entry->name = (char*)malloc(strlen(str));
    if (entry->name == NULL) {
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
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_depends(PyObject* module, mc_plugin_entry_t* entry) {

    PyObject* func=NULL, *value=NULL;
    char* str;

    func = PyObject_GetAttrString(module, PLUGIN_DEPS_FUNC);
    if (func == NULL || PyCallable_Check(func) == 0) {
        PyErr_Print();
        printf("Couldn't find entry point.\n");
        return -1;
    }

    value = PyObject_CallObject(func, NULL);
    Py_DECREF(func);
    if (value == NULL) {
        PyErr_Print();
        printf("Couldn't call entry point.\n");
        return -1;
    }

    str = PyUnicode_AsUTF8(value);
    entry->name = (char*)malloc(strlen(str));
    if (entry->name == NULL) {
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
 * @return Returns 0 on success, negative value on error.
 * @internal
 */
int get_plugin_entry(PyObject* module, mc_plugin_entry_t* entry) {

    PyObject* func=NULL, *value=NULL;
    char* str;

    func = PyObject_GetAttrString(module, PLUGIN_ENTRY_FUNC);
    if (func == NULL || PyCallable_Check(func) == 0) {
        PyErr_Print();
        printf("Couldn't find entry point.\n");
        return -1;
    }

    value = PyObject_CallObject(func, NULL);
    Py_DECREF(func);
    if (value == NULL) {
        PyErr_Print();
        printf("Couldn't call entry point.\n");
        return -1;
    }

    str = PyUnicode_AsUTF8(value);
    entry->name = (char*)malloc(strlen(str));
    if (entry->name == NULL) {
        return -1;
    }
    strcpy(entry->name, str);

    Py_DECREF(value);
    return 0;
}

/**
 * Loads a plugin.  This must point to the plugin.py file, or whatever file has
 * the various information functions.
 *
 * @param path Plugin file or directory.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_load(const char* path) {

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
    rc = get_plugin_name(entry->module, entry);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_version(entry->module, entry);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_depends(entry->module, entry);
    if (rc < 0) {
        return rc;
    }

    /* */
    rc = get_plugin_entry(entry->module, entry);
    if (rc < 0) {
        return rc;
    }

    //TODO: run plugin.py file, this should contain init code
    //      if it fails to run, free it here

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
 *
 * @return Returns 0 on success, negative value on error.
 */
int mc_plugin_unload(const char* name) {

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
