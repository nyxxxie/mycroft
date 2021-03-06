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
#include <Python.h>
#include <mycroft/script.h>
#include "script.h"
#include "config.h"

#define INITSCRIPT "init.py"

mc_error_t mc_script_runfile(const char* path)
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
