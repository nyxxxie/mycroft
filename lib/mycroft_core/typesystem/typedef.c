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

#include <stdlib.h>
#include <stdio.h>
#include <mycroft/error.h>
#include "typedef.h"

mc_typedef_t* mc_typedef_create()
{
    mc_typedef_t* ret = NULL;

    /* Allocate type definition */
    ret = (mc_typedef_t*)malloc(sizeof(*ret));
    if (ret == NULL) {
        MC_ERROR("Failed to allocate type definition.\n");
        return ret;
    }

    /* Zero all members */
    ret->name = NULL;
    ret->size = 0;
    ret->to_string_fn = NULL;
    ret->from_string_fn = NULL;
}

void mc_typedef_free(mc_typedef_t* type)
{
    if (type->name != NULL) {
        free(type->name);
    }

    free(type);
}

mc_error_t mc_typedef_set_name(mc_typedef_t* type, const char* name)
{
    int name_len = strlen(name);

    type->name = (char*)realloc(type->name, name_len + 1);
    if (type->name == NULL) {
        MC_ERROR("Failed to (re)allocate type->name.\n");
        return MC_ERR;
    }
    memset(type->name, 0, name_len + 1);
    strncpy(type->name, name, name_len);

    return MC_OK;
}

mc_error_t mc_typedef_to_string(mc_typedef_t* type, uint8_t* bytes, size_t byte_amount, char** string, size_t* string_size)
{
    /* Check to make sure function is defined */
    if (type->to_string_fn == NULL) {
        MC_ERROR("to_string_fn is NULL.\n");
        return MC_ERR;
    }

    return (*type->to_string_fn)(type, bytes, byte_amount, string, string_size);
}

mc_error_t mc_typedef_from_string(mc_typedef_t* type, const char* string, uint8_t** bytes, size_t* byte_amount)
{
    /* Check to make sure function is defined */
    if (type->from_string_fn == NULL) {
        MC_ERROR("to_string_fn is NULL.\n");
        return MC_ERR;
    }

    return (*type->from_string_fn)(type, string, bytes, byte_amount);
}
