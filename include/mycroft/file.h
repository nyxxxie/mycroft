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

#ifndef MYCROFT_FILE_H
#define MYCROFT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

/* #include <stdbool.h> */
#include <stdio.h>
#include <stdint.h>
#include <mycroft/template.h>
#include <mycroft/error.h>

/** Size used by internal file functions */
typedef off_t fsize_t;

typedef struct mc_file_t mc_file_t;

/* Util functions */
int mc_file_access(const char* file_name);

/* Init/free functions */
mc_file_t* mc_file_open(const char* file_name);
mc_error_t mc_file_close(mc_file_t* file);
/*
 * NOTE: not implemented
 * int mc_file_reload(mc_file_t* file);
 */

/* Template functions */
template_t* mc_file_get_template(mc_file_t* file);
mc_error_t mc_file_set_template(mc_file_t* file, template_t* t);

/* Management functions */
fsize_t mc_file_get_cursor(mc_file_t* file);
mc_error_t mc_file_set_cursor(mc_file_t* file, fsize_t cursor);
fsize_t mc_file_size(mc_file_t* file);
char* mc_file_name(mc_file_t* file);
char* mc_file_path(mc_file_t* file);

/* Cache functions */
mc_error_t mc_file_cache_init(mc_file_t* file, fsize_t size);
mc_error_t mc_file_cache_loadzone(mc_file_t* file, fsize_t cursor);
mc_error_t mc_file_cache_reload(mc_file_t* file);

/* Read/write functions */
mc_error_t mc_file_read(mc_file_t* file, fsize_t amount,
                        uint8_t* outbuf, fsize_t* amount_read);
mc_error_t mc_file_read_raw(mc_file_t* file, fsize_t amount,
                            uint8_t* outbuf, fsize_t* amount_read);
mc_error_t mc_file_write(mc_file_t* file, fsize_t amount,
                         uint8_t* outbuf, fsize_t* amount_written);
mc_error_t mc_file_read_value(mc_file_t* file, fsize_t offset,
                              fsize_t amount, uint8_t* outbuf,
                              fsize_t* amount_read);
mc_error_t mc_file_write_value(mc_file_t* file, fsize_t offset,
                               fsize_t amount, uint8_t* outbuf,
                               fsize_t* amount_written);
/*
 * NOTE: not implemented
 * mc_error_t mc_file_write_insert(mc_file_t* file, fsize_t offset,
 *                                 fsize_t amount, uint8_t* outbuf,
 *                                 fsize_t* amount_written);
 */
mc_error_t mc_file_read_cache(mc_file_t* file, fsize_t offset,
                              fsize_t amount, uint8_t* outbuf,
                              fsize_t* amount_read);
/*
 * NOTE: not implemented
 * mc_error_t mc_file_read_editor_area(mc_file_t* file, fsize_t offset,
 *                                     fsize_t amount, uint8_t* outbuf,
 *                                     fsize_t* amount_read);
 */

/* Misc functions */
mc_error_t mc_file_hash(mc_file_t* file, uint8_t** hash, size_t* hashlen);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_FILE_H
