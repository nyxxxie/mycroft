#include <stdio.h>
#include <wchar.h>
#include <Python.h>
#include "binds/file.h"
#include "binds/context.h"
#include "context.h"
#include "script.h"
#include "config.h"

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

mc_error_t mc_interpreter_runinitscript(mc_interpreter_t* i)
{
    size_t path_size = 0;
    char* path = NULL;

    if (i->bindings_initialized >= 2) {
        MC_ERROR("You've already run the init script.\n");
        return MC_ERR;
    }
    else if (i->bindings_initialized < 1) {
        MC_ERROR("Please run stage 1 initialization before this function.\n");
        return MC_ERR;
    }

    /* Calculate size of the script path */
    path_size = strlen(MYCROFT_INSTALL_SCRIPT_PATH)
        + strlen("/")
        + strlen(INITSCRIPT)
        + 1;

    /* Allocate space for the script path */
    path = (char*)calloc(path_size, 1);
    if (path == NULL) {
        MC_ERROR("Failed to calloc space for script path.\n");
        return MC_ERR;
    }

    /* Piece together the path to the init script */
    strcpy(path, MYCROFT_INSTALL_SCRIPT_PATH);
    strcat(path, "/");
    strcat(path, INITSCRIPT);

    /* Run the init script */
    if (mc_script_runfile(i, path) != MC_OK) {
        MC_ERROR("Failed to run init script.\n");
        return MC_ERR;
    }
    MC_DEBUG("Ran init script.\n");

    /* Mark that we've run the init script */
    i->bindings_initialized = 2;

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
