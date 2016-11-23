#include <stdio.h>
#include <Python.h>
#include "context.h"
#include "script.h"

mc_error_t mc_script_runfile(mc_ctx_t* ctx, const char* path)
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

mc_error_t mc_script_runstring(mc_ctx_t* ctx, const char* script)
{
    int rc = 0;

    /* Run script */
    rc = PyRun_SimpleString(script);
    if (rc < 0) {
        return MC_ERR;
    }

    return MC_OK;
}
