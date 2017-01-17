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

#include <stdint.h>
#include <stdlib.h>
#include "hook.h"

int hook_init()
{
    int rc = 0;

    mc_hooks = NULL; // Indicate that there are no entries.

    for (size_t i; i < MC_HOOK_BUILTIN_TOTAL; i++) {
        rc = hook_event_create(default_hooks[i]);
        if (rc < 0) {
            return rc;
        }
    }

    return 0;
}

int hook_free()
{
    int rc = 0;
    mc_hook_t* cur = NULL;

    /* Loop through each hook */
    cur = mc_hooks;
    while (cur != NULL) {
        mc_hook_t* next = cur->next;
        mc_hook_reciever_t* rcur = NULL;

        /* Delete each reciever in the hook */
        while (rcur != NULL) {
            mc_hook_reciever_t* rnext = rcur->next;

            /* Delete the reciever */
            free(rcur->name);
            free(rcur);

            /* Move on */
            rcur = rnext;
        }

        /* Delete the hook */
        free(cur);

        /* Move on */
        cur = next;
    }

    return rc;
}

int hook_event_create(mc_hook_id_t hook_id)
{
    int rc = 0;

    /* Search for hook_id and error if it exists */

}

int hook_event_remove(mc_hook_id_t hook_id)
{
    int rc = 0;

    /* Search for hook_id and delete it if it exists */
}

int hook_add(mc_hook_id_t hook_id, const char* hook_name, mc_hook_cb_t* callback)
{
    int rc = 0;

    /* Find hook id */

    /* See if hook_name exists and replace the previous callback if so */

    /* Insert hook into chain */
}

int hook_remove(mc_hook_id_t hook_id, const char* hook_name)
{
    int rc = 0;

    /* Find hook id */

    /* Remove hook when the identifier is found */

}

int hook_dispatch(mc_hook_id_t hook_id, mc_hook_data_t* args)
{
    int rc = 0;

    /* Find hook */

    /* Call each of the reciever callbacks */

}
