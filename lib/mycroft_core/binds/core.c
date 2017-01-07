#include <Python.h>
#include <mycroft/mycroft.h>
#include "binds/file.h"
#include "binds/project.h"

//static PyObject* mfile_exists(PyObject* self, PyObject* args) {
//    int rc = 0;
//    const char* fname = NULL;
//
//    /* Get arguments */
//    if (!PyArg_ParseTuple(args, "s", &fname)) {
//        return NULL;
//    }
//
//    printf("DEBUG: does %s exist? ", fname);
//
//    rc = file_exists(fname);
//    if (rc < 0) {
//        Py_RETURN_FALSE;
//    }
//
//    Py_RETURN_TRUE;
//}

static PyObject* mcore_add_project(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_remove_project(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_get_project(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_get_project_amount(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_set_focused_project(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyObject* mcore_get_focused_project(PyObject* self, PyObject* args)
{
    Py_RETURN_TRUE;
}

static PyMethodDef mcore_methods[] = {
    {"add_project",    (PyCFunction)mcore_add_project,    METH_VARARGS, ""},
    {"remove_project", (PyCFunction)mcore_remove_project, METH_VARARGS, ""},
    {"get_project",    (PyCFunction)mcore_get_project,    METH_VARARGS, ""},
    {"get_project_amount",  (PyCFunction)mcore_get_project_amount,  METH_VARARGS, ""},
    {"set_focused_project", (PyCFunction)mcore_set_focused_project, METH_VARARGS, ""},
    {"get_focused_project", (PyCFunction)mcore_get_focused_project, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef mcore_module = {
    PyModuleDef_HEAD_INIT,
    "mcore",
    "Bindings for mcore main api.",
    -1,
    mcore_methods,
    NULL, NULL, NULL, NULL
};

static PyMODINIT_FUNC PyInit_mcore()
{
    PyObject* m = NULL;

    /* Create module */
    m = PyModule_Create(&mcore_module);
    if (m == NULL) {
        return NULL;
    }

    return m;
}

int init_binds_core()
{
    int rc = 0;

    rc = PyImport_AppendInittab("mcore", PyInit_mcore);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
