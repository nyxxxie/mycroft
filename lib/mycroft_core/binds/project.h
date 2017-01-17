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

#ifndef BINDS_PROJECT_H
#define BINDS_PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <mycroft/project.h>

typedef struct {
    PyObject_HEAD
    mc_project_t* project;
} mproject_data_t;

PyObject* create_mproject(mc_project_t* project);

int init_binds_project();

#ifdef __cplusplus
}
#endif

#endif // BINDS_PROJECT_H
