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

#ifndef MYCROFT_INT_SCRIPT_H
#define MYCROFT_INT_SCRIPT_H

#include <mycroft/error.h>

/**
 * Represents a script
 */
typedef struct {
    int bindings_initialized;
} mc_interpreter_t;

mc_interpreter_t* mc_interpreter_create();
void mc_interpreter_free(mc_interpreter_t* i);

mc_error_t mc_interpreter_add_path(mc_interpreter_t* i, const char* directory);
mc_error_t mc_interpreter_run_init_scripts(mc_interpreter_t* i);

/** Global interpreter instance.  This is global because there can only be one python intepreter instance. */
extern mc_interpreter_t* _interpreter;
#define MC_INTERPRETER_START \
    if (_interpreter == NULL) { \
        _interpreter = mc_interpreter_create(); \
    }

#define MC_INTERPRETER_END \
    if (_interpreter != NULL) { \
        mc_interpreter_free(_interpreter); \
    }

#endif // MYCROFT_INT_SCRIPT_H
