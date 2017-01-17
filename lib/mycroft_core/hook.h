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

#ifndef MYCROFT_INT_HOOK_H
#define MYCROFT_INT_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/hook.h>

/**
 * Represents a hook reciever.  A hook reciever contains a reciever registered
 * by hook_add to listen for a specific event.
 */
typedef struct mc_hook_reciever_t mc_hook_reciever_t;
struct mc_hook_reciever_t {
    char* name;     /**< @internal String that identifies the hook */
    void* callback; /**< @internal */
    mc_hook_reciever_t* prev; /**< @internal */
    mc_hook_reciever_t* next; /**< @internal */
};

/**
 * Represents a hook.
 */
typedef struct mc_hook_t mc_hook_t;
struct mc_hook_t {
    mc_hook_reciever_t* recievers; /**< @internal */
    mc_hook_id_t id;   /**< @internal */
    mc_hook_t*   prev; /**< @internal */
    mc_hook_t*   next; /**< @internal */
};

/**
 * @internal
 * Start to linked list containing all hooks.
 */
static mc_hook_t* mc_hooks;

/**
 * @internal
 * Built-in hooks.
 */
const char* default_hooks[] = {
    "INIT_PRE",
    "INIT_POST"
};
#define MC_HOOK_BUILTIN_TOTAL sizeof(default_hooks)/sizeof(*default_hooks)

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_HOOK_H
