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

#ifndef MYCROFT_INT_PLUGIN_H
#define MYCROFT_INT_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <mycroft/plugin.h>

/**
 * Represents a plugin that is currently loaded.
 */
struct mc_plugin_entry_t {
    char* name;
    char* version;
    PyObject* module;
    PyObject* entryfunc;
    mc_plugin_entry_t* next;
    mc_plugin_entry_t* prev;
};

// TODO: remove these and place them in mc_ctx_t or something
static mc_plugin_entry_t* plugin_first = NULL;
static mc_plugin_entry_t* plugin_last = NULL;

int mc_plugin_entry_init(mc_plugin_entry_t** entry);
int mc_plugin_entry_free(mc_plugin_entry_t* entry);

#define PLUGIN_ENTRY_FUNC "_mc_entry"
#define PLUGIN_NAME_FUNC  "_mc_name"
#define PLUGIN_VER_FUNC   "_mc_version"
#define PLUGIN_DEPS_FUNC  "_mc_depends"

#define PLUGIN_INIT_FILE  "__init__.py"

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_PLUGIN_H
