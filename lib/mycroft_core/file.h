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

#ifndef MYCROFT_INT_FILE_H
#define MYCROFT_INT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <mycroft/file.h>

/**
 * Represents the buffer used by the editor to display bytes.  We read a
 * little extra of the file in order to try and minimise disk access.
 */
typedef struct {
    uint8_t* buf;   /**< @internal */
    fsize_t  size;  /**< @internal */
    fsize_t  base;  /**< @internal */
} file_cache_t;

/**
 * Contains state information for an open file.  Don't try to use any of these
 * fields, they may change in the future.
 */
struct mc_file_t {
    char* name;     /**< @internal */
    char* path;     /**< @internal */
    FILE* fp;       /**< @internal */
    fsize_t size;   /**< @internal */
    fsize_t cursor; /**< @internal */
    template_t* t;  /**< @internal */
    file_cache_t* cache; /**< @internal */
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_FILE_H
