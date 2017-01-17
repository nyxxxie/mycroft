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

#ifndef MYCROFT_PROJECT_H
#define MYCROFT_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/file.h>
#include <mycroft/error.h>

typedef struct mc_project_t mc_project_t;

/* Create/free */
mc_project_t* mc_project_create(const char* name);
mc_project_t* mc_project_load(const char* mdb_file);
mc_error_t mc_project_save(mc_project_t* project, const char* mdb_file);
void mc_project_free(mc_project_t* project);

/* Project info */
mc_error_t mc_project_set_name(mc_project_t* project, const char* name);
char* mc_project_get_name(mc_project_t* project);

/* File management */
mc_error_t mc_project_add_file(mc_project_t* project, mc_file_t* file);
mc_error_t mc_project_remove_file(mc_project_t* project, uint32_t file_index);
mc_file_t* mc_project_get_file(mc_project_t* project, uint32_t file_index);
uint32_t mc_project_get_file_amount(mc_project_t* project);
mc_error_t mc_project_set_focused_file(mc_project_t* project, mc_file_t* file);
mc_file_t* mc_project_get_focused_file(mc_project_t* project);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PROJECT_H
