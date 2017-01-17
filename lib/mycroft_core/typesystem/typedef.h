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

#ifndef TYPEDEF_H
#define TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>
#include <mycroft/error.h>

typedef struct mc_typedef_t mc_typedef_t;

typedef mc_error_t (*to_string_fn_t)(mc_typedef_t*, uint8_t*, size_t, char**, size_t*);
typedef mc_error_t (*from_string_fn_t)(mc_typedef_t*, const char*, uint8_t**, size_t*);

struct mc_typedef_t {
    char* name;
    uint64_t size;
    to_string_fn_t to_string_fn;
    from_string_fn_t from_string_fn;
};

mc_typedef_t* mc_typedef_create();
void mc_typedef_free(mc_typedef_t* type);

mc_error_t mc_typedef_set_name(mc_typedef_t* type, const char* name);

mc_error_t mc_typedef_to_string(mc_typedef_t* type, uint8_t* bytes, size_t byte_amount, char** string, size_t* string_size);
mc_error_t mc_typedef_from_string(mc_typedef_t* type, const char* string, uint8_t** bytes, size_t* byte_amount);

#ifdef __cplusplus
}
#endif

#endif // TYPEDEF_H
