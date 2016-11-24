#include <stdio.h>
#include <Python.h>
#include "binds/file.h"
#include "binds/context.h"
#include "context.h"
#include "script.h"

mc_error_t init_bindings()
{
    int rc = 0;

    rc = init_binds_file();
    if (rc < 0) {
        return MC_ERR;
    }

    //rc = init_binds_core();
    //if (rc < 0) {
    //    return rc;
    //}

    return MC_OK;
}

mc_interpreter_t* mc_interpreter_create()
{
    mc_interpreter_t* ret = NULL;

    /* Alloc the project */
    ret = (mc_interpreter_t*)malloc(sizeof(*ret));
    if (ret == NULL) {
        MC_ERROR("Failed to allocate interpreter ctx.\n");
        return NULL;
    }
    ret->bindings_initialized = 0;

    /* */
    if (init_bindings() != MC_OK) {
        MC_ERROR("Failed to initialize bindings.\n");
        free(ret);
        return NULL;
    }
    ret->bindings_initialized = 1;

    Py_Initialize();

    return ret;
}

void mc_interpreter_free(mc_interpreter_t* i)
{
    Py_Finalize();
    free(i);
}

mc_error_t mc_script_runfile(mc_interpreter_t* i, const char* path)
{
    int rc = 0;
    FILE* file = NULL;;

    /* Open file */
    file = fopen(path, "r");
    if (file == NULL) {
        MC_ERROR("Failed to open file \"%s\": %s\n", path, strerror(errno));
        return MC_ERR;
    }

    /* Run script */
    rc = PyRun_SimpleFileEx(file, path, 1);
    if (rc < 0) {
        PyErr_Print();
        MC_ERROR("Failed to run python script \"%s\".\n", path);
        return MC_ERR;
    }

    return MC_OK;
}

mc_error_t mc_script_runstring(mc_interpreter_t* i, const char* script)
{
    int rc = 0;

    /* Run script */
    rc = PyRun_SimpleString(script);
    if (rc < 0) {
        return MC_ERR;
    }

    return MC_OK;
}
