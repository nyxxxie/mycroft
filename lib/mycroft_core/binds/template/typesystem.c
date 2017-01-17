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

#include "typesystem/typesystem.h"
#include "binds/template/typesystem.h"

static PyObject* mcore_typesystem_add_type(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_typesystem_create_type(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyMethodDef mcore_typesystem_methods[] = {
    {"add_type",    (PyCFunction)mcore_typesystem_add_type,    METH_VARARGS, ""},
    {"create_type", (PyCFunction)mcore_typesystem_create_type, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef mcore_typesystem_module = {
    PyModuleDef_HEAD_INIT,
    "mcore_typesystem",
    "Bindings for mycroft typesystem api.",
    -1,
    mcore_typesystem_methods,
    NULL, NULL, NULL, NULL
};

static PyMODINIT_FUNC PyInit_mcore_typesystem() {
    PyObject* m = NULL;

    /* Create module */
    m = PyModule_Create(&mcore_typesystem_module);
    if (m == NULL) {
        return NULL;
    }

    return m;
}

int init_binds_typesystem() {
    int rc = 0;

    rc = PyImport_AppendInittab("mcore_typesystem", PyInit_mcore_typesystem);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
