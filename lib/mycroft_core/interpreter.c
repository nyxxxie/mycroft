#include <stdio.h>
#include <wchar.h>
#include <Python.h>
#include "interpreter.h"
#include "script.h"
#include "binds/core.h"
#include "binds/project.h"
#include "binds/file.h"

mc_interpreter_t* _interpreter = NULL;

mc_error_t init_bindings()
{
    int rc = 0;

    rc = init_binds_core();
    if (rc < 0) {
        MC_ERROR("Failed to initialize core bindings.\n");
        return MC_ERR;
    }

    rc = init_binds_project();
    if (rc < 0) {
        MC_ERROR("Failed to initialize file bindings.\n");
        return MC_ERR;
    }

    rc = init_binds_file();
    if (rc < 0) {
        MC_ERROR("Failed to initialize file bindings.\n");
        return MC_ERR;
    }

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

mc_error_t mc_interpreter_add_path(mc_interpreter_t* i, const char* directory)
{
    wchar_t* dir_conv = NULL;
    wchar_t* curpath = NULL;
    wchar_t* newpath = NULL;
    size_t newpath_size = 0;
    #ifdef __linux__
    const wchar_t* separator = L":";
    #else
    const char* separator = L";";
    #endif

    /* Convert directory argument to wchat_t */
    dir_conv = (wchar_t*)calloc(sizeof(*dir_conv) * (strlen(directory) + 1), 1);
    swprintf(dir_conv, strlen(directory) + 1, L"%hs", directory);

    /* Get current path */
    curpath = Py_GetPath();
    if (curpath == NULL) {
        MC_ERROR("Bad path returned by Py_GetPath\n");
        return MC_ERR;
    }

    newpath_size = wcslen(curpath) + wcslen(separator) + wcslen(dir_conv) + 1;
    newpath_size *= sizeof(*newpath);
    newpath = (wchar_t*)calloc(newpath_size, 1);
    if (newpath == NULL) {
        MC_ERROR("Failed to calloc space for path.\n");
        return MC_ERR;
    }
    memset(newpath, 0, newpath_size); // set all to null for safety

    /* Concat to create new string */
    wcscpy(newpath, curpath);
    wcscat(newpath, separator);
    wcscat(newpath, dir_conv);

    /* Add to path */
    PySys_SetPath(newpath);
}
