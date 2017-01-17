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

#ifndef MYCROFT_INT_CORE_H
#define MYCROFT_INT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mycroft.h>
#include <mycroft/project.h>
#include "interpreter.h"
#include "typesystem/typesystem.h"

/** @internal Contains global data for mycroft.  Meant to be accessed using mc functions, don't access this yourself. */
typedef struct {
    mc_project_t**    projects;
    uint32_t          project_amt;
    mc_project_t*     project_focused;
    mc_interpreter_t* interpreter;
    mc_typesystem_t*  typesystem;
} mc_ctx_t;

/** @internal Global internal ctx instance.  Don't use this directly outside of mycroft.c! */
extern mc_ctx_t* _ctx;

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_CORE_H
