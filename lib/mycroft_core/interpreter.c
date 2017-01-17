/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <stdio.h>
#include <wchar.h>
#include <Python.h>
#include <mycroft/script.h>
#include "interpreter.h"
#include "config.h"
#include "binds/core.h"
#include "binds/project.h"
#include "binds/file.h"
#include "binds/template/typesystem.h"

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
        MC_ERROR("Failed to initialize project bindings.\n");
        return MC_ERR;
    }

    rc = init_binds_file();
    if (rc < 0) {
        MC_ERROR("Failed to initialize file bindings.\n");
        return MC_ERR;
    }

    rc = init_binds_typesystem();
    if (rc < 0) {
        MC_ERROR("Failed to initialize typesystem bindings.\n");
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
    PyObject *pypath=NULL, *localname=NULL;

    MC_DEBUG("Adding \"%s\" to python path.\n", directory);

    pypath = PySys_GetObject("path");
    localname = PyUnicode_FromString(directory);

    PyList_Append(pypath, localname);

    Py_DECREF(localname);

    return MC_OK;

}

mc_error_t mc_interpreter_run_init_scripts(mc_interpreter_t* i)
{
    const char* init_script = MYCROFT_INSTALL_SCRIPT_PATH "/init.py";
    mc_error_t rc;

    /* */
    rc = mc_script_runfile(init_script);
    if (rc != MC_OK) {
        MC_ERROR("Failed to run init script \"%s\".\n", init_script);
        return rc;
    }

    return MC_OK;
}
