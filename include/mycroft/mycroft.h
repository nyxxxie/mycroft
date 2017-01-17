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

#ifndef MYCROFT_H
#define MYCROFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <mycroft/project.h>

/** Expected file extension for a mycroft template file */
#define MYCROFT_TEMPLATE_EXTENSION "mtf"

/** Expected file extension for a mycroft project file */
#define MYCROFT_PROJECT_EXTENSION "mpf"

/* Create/free */
mc_error_t mc_init();
mc_error_t mc_init_scripting(const char* path[], size_t amount);
void mc_destroy();

/* Context management */
mc_error_t mc_add_project(mc_project_t* project);
mc_error_t mc_remove_project(uint32_t project_index);
mc_project_t* mc_get_project(uint32_t project_index);
uint32_t mc_get_project_amount();
mc_error_t mc_set_focused_project(mc_project_t* project);
mc_project_t* mc_get_focused_project();

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_H
