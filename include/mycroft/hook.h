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

#ifndef MYCROFT_HOOK_H
#define MYCROFT_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif

/** Indicates the type of a callback function. */
typedef enum {
    UNKNOWN = 0,
    C,
    PYTHON
} mc_cb_type_t;

/** */
typedef unsigned int mc_hook_id_t;

/**
 * Stores a hook callback function.  Since callback functions can be both
 * python objects or c functions, we can't just store a pointer to a function.
 * This struct contains both the pointer of the callback handler and also the
 * type of handler, which indicates to hook_dispatch as to how to handle it.
 */
typedef struct {
    void*        func;
    mc_cb_type_t func_type;
} mc_hook_cb_t;

/**
 * Contains the data passed to a hook callback.  Data will be freed after the
 * callback returns, so don't try and use this struct or any data in it
 * after it is freed.  If you want to use it later outside of the callback,
 * make a copy.
 */
typedef struct {
    int   hook_id;
    void* argv;
    int   argc;
} mc_hook_data_t;

/** Expected type of a C callback function. */
typedef void (*mc_hook_cbfunc_t)(mc_hook_data_t args);


int hook_init();
int hook_free();

int hook_event_create(mc_hook_id_t hook_id);
int hook_event_remove(mc_hook_id_t hook_id);

int hook_add(mc_hook_id_t hook_id, const char* hook_name, mc_hook_cb_t* callback);
int hook_remove(mc_hook_id_t hook_id, const char* hook_name);

int hook_dispatch(mc_hook_id_t hook_id, mc_hook_data_t* args);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_HOOK_H
