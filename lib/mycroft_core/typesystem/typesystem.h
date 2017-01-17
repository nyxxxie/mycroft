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

#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "typesystem/typedef.h"

typedef struct {
    mc_typedef_t** typedefs;
    size_t typedef_amount;
} mc_typesystem_t;

mc_typesystem_t* mc_typesystem_create();
void mc_typesystem_free(mc_typesystem_t* ts);

mc_error_t mc_typesystem_register_type(mc_typesystem_t* td, mc_typedef_t* type_new);
mc_error_t mc_typesystem_create_type(mc_typesystem_t* ts, const char* name, uint64_t size, to_string_fn_t to_string, from_string_fn_t from_string);

mc_typedef_t* mc_typesystem_get_type(mc_typesystem_t* ts, const char* type_name);

#ifdef __cplusplus
}
#endif

#endif // TYPESYSTEM_H
