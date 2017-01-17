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

#ifndef MYCROFT_ERROR_H
#define MYCROFT_ERROR_H

#include <errno.h>

typedef enum {
    MC_OK                = 0,
    MC_ERR               = -1,
    MC_FILE_ERR_FILENAME = -2,
    MC_FILE_ERR_MALLOC   = -3,
    MC_FILE_ERR_IO       = -4,
    MC_FILE_ERR_BOUNDS   = -5,
} mc_error_t;

// Optional arguments
#define __OARGS(...) , ##__VA_ARGS__

// Generic error output function
#define MC_ERROR(fmt, ...) \
    fprintf(stderr, "ERROR: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stderr, fmt __OARGS(__VA_ARGS__));

// Generic debug output function
#define MC_DEBUG(fmt, ...) \
    fprintf(stdout, "DEBUG: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stdout, fmt __OARGS(__VA_ARGS__));

#endif // MYCROFT_ERROR_H
