#include <Python.h>
#include <mycroft/project.h>
#include <mycroft/file.h>
#include "binds/project.h"
#include "binds/file.h"

static PyObject* mproject_name(mproject_data_t* self, PyObject* dontuse)
{
    if (self->project == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "object has no project");
        return NULL;
    }

    return PyUnicode_FromString("asdfasfd");
}

static PyObject* mproject_create(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_load(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_save(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_free(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_set_name(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_get_name(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_add_file(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_remove_file(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_get_file(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_get_file_amount(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_set_focused_file(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mproject_get_focused_file(mproject_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyMethodDef mproject_methods[] = {
    {"create", (PyCFunction)mproject_create, METH_NOARGS, ""},
    {"load", (PyCFunction)mproject_load, METH_NOARGS, ""},
    {"save", (PyCFunction)mproject_save, METH_NOARGS, ""},
    {"free", (PyCFunction)mproject_free, METH_NOARGS, ""},
    {"set_name", (PyCFunction)mproject_set_name, METH_NOARGS, ""},
    {"get_name", (PyCFunction)mproject_get_name, METH_NOARGS, ""},
    {"add_file", (PyCFunction)mproject_add_file, METH_NOARGS, ""},
    {"remove_file", (PyCFunction)mproject_remove_file, METH_NOARGS, ""},
    {"get_file", (PyCFunction)mproject_get_file, METH_NOARGS, ""},
    {"get_file_amount", (PyCFunction)mproject_get_file_amount, METH_NOARGS, ""},
    {"set_focused_file", (PyCFunction)mproject_set_focused_file, METH_NOARGS, ""},
    {"get_focused_file", (PyCFunction)mproject_get_focused_file, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyObject* mproject_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    mproject_data_t* self = NULL;

    /* Allocate space for the data struct */
    self = (mproject_data_t*)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }

    /* Set defaults */
    self->project = NULL;

    return (PyObject*)self;
}

static PyObject* mproject_dealloc(mproject_data_t* self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int mproject_init(mproject_data_t* self, PyObject* args, PyObject* kwds)
{
    return 0;
}

#define MPROJECT_DOC "It's a project!"

static PyTypeObject mproject_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mproject",                /* tp_name */
    sizeof(mproject_data_t),   /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    MPROJECT_DOC,              /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    mproject_methods,          /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    mproject_new,              /* tp_new */
};

static PyObject* mcore_project_what(PyObject* self, PyObject* args)
{
    printf("RAN A THING OHHMMEEEHGGEEEEEE");

    Py_RETURN_TRUE;
}

static PyMethodDef mcore_project_methods[] = {
    {"what",  (PyCFunction)mcore_project_what,  METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

PyObject* create_mproject(mc_project_t* project)
{
    mproject_data_t* ret = NULL;

    /* Create object */
    ret = PyObject_New(mproject_data_t, &mproject_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Initialize object */
    ret = (mproject_data_t*)PyObject_Init((PyObject*)ret, &mproject_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Set project */
    ret->project = project;

    return (PyObject*)ret;
}

static PyModuleDef mcore_project_module =
{
    PyModuleDef_HEAD_INIT,
    "mcore_project",
    "Bindings for mcore's project api.",
    -1,
    mcore_project_methods,
    NULL, NULL, NULL, NULL
};

static PyMODINIT_FUNC PyInit_mcore_project()
{
    PyObject* m = NULL;

    /* Make sure the type is ready to go */
    if (PyType_Ready(&mproject_type) < 0) {
        return NULL;
    }

    /* Create module */
    m = PyModule_Create(&mcore_project_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&mproject_type);
    PyModule_AddObject(m, "mproject", (PyObject*)&mproject_type);

    return m;
}

int init_binds_project()
{
    int rc = 0;

    rc = PyImport_AppendInittab("mcore_project", PyInit_mcore_project);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
