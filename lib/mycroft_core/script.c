#include <stdio.h>
#include <Python.h>
#include <mycroft/script.h>
#include "script.h"
#include "config.h"

#define INITSCRIPT "init.py"

mc_error_t mc_script_runfile(const char* path)
{
//    int rc = 0;
//    FILE* fp = NULL;
//    PyObject* v = NULL;
//    PyObject* vars = NULL;
//    PyObject* pyctx = NULL;
//    PyObject* module = NULL;
//
//    /* Run module */
//    fp = fopen(path, "r");
//    if (fp == NULL) {
//        MC_ERROR("Failed to open file \"%s\": %s\n", path, strerror(errno));
//        return MC_ERR;
//    }
//
//    /* Create module to run */
//    module = PyImport_AddModule("__main__");
//    if (module == NULL) {
//        PyErr_Print();
//        MC_ERROR("Couldn't create dict.\n");
//        fclose(fp);
//        return MC_ERR;
//    }
//    Py_INCREF(module);
//
//    /* Create ctx object */
//    pyctx = create_mcore_ctx(_ctx);
//    if (pyctx == NULL) {
//        MC_ERROR("Failed to create a python ctx object.\n");
//        fclose(fp);
//        Py_DECREF(module);
//        return MC_ERR;
//    }
//
//    /* Create dict */
//    vars = PyModule_GetDict(module);
//    if (vars == NULL) {
//        PyErr_Print();
//        MC_ERROR("Couldn't create dict.\n");
//        fclose(fp);
//        Py_DECREF(module);
//        Py_DECREF(pyctx);
//        return MC_ERR;
//    }
//
//    /* Add vars to module var dict */
//    if (PyDict_SetItemString(vars, "ctx", pyctx) < 0) {
//        PyErr_Print();
//        MC_ERROR("Failed to set ctx item in var dict.\n");
//        fclose(fp);
//        Py_DECREF(module);
//        Py_DECREF(pyctx);
//        return MC_ERR;
//    }
//
//    /* Run script and handle any exceptions */
//    v = PyRun_FileExFlags(fp, path, Py_file_input, vars, vars, 1, NULL);
//    if (v == NULL) {
//        PyErr_Print();
//        MC_ERROR("Caught unhandled exception in python file.\n");
//        fclose(fp);
//        Py_DECREF(module);
//        Py_DECREF(pyctx);
//        return MC_ERR;
//    }
//
//    Py_DECREF(module);

    return MC_OK;
}

mc_error_t mc_script_runstring(const char* script)
{
    int rc = 0;

    /* Run script */
    rc = PyRun_SimpleString(script);
    if (rc < 0) {
        return MC_ERR;
    }

    return MC_OK;
}

//mc_error_t mc_interpreter_runinitscript(mc_interpreter_t* i)
//{
//    size_t path_size = 0;
//    char* path = NULL;
//
//    if (i->bindings_initialized >= 2) {
//        MC_ERROR("You've already run the init script.\n");
//        return MC_ERR;
//    }
//    else if (i->bindings_initialized < 1) {
//        MC_ERROR("Please run stage 1 initialization before this function.\n");
//        return MC_ERR;
//    }
//
//    /* Calculate size of the script path */
//    path_size = strlen(MYCROFT_INSTALL_SCRIPT_PATH)
//        + strlen("/")
//        + strlen(INITSCRIPT)
//        + 1;
//
//    /* Allocate space for the script path */
//    path = (char*)calloc(path_size, 1);
//    if (path == NULL) {
//        MC_ERROR("Failed to calloc space for script path.\n");
//        return MC_ERR;
//    }
//
//    /* Piece together the path to the init script */
//    strcpy(path, MYCROFT_INSTALL_SCRIPT_PATH);
//    strcat(path, "/");
//    strcat(path, INITSCRIPT);
//
//    /* Run the init script */
//    if (mc_script_runfile(i, path) != MC_OK) {
//        MC_ERROR("Failed to run init script.\n");
//        return MC_ERR;
//    }
//    MC_DEBUG("Ran init script.\n");
//
//    /* Mark that we've run the init script */
//    i->bindings_initialized = 2;
//
//    return MC_OK;
//}
