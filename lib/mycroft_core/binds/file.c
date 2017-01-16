#include <Python.h>
#include <mycroft/file.h>
#include "binds/file.h"

//static PyObject* mfile_name(mfile_data_t* self, PyObject* dontuse) {
//    if (self->file == NULL) {
//        PyErr_SetString(PyExc_RuntimeError, "object has no file");
//        return NULL;
//    }
//
//    return PyUnicode_FromString(mc_file_name(self->file));
//}

static PyObject* mfile_reload(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_get_template(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_set_template(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_get_cursor(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_set_cursor(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_size(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_name(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_path(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_cache_init(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_cache_loadzone(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_cache_reload(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_read(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_read_raw(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_write(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_read_value(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_write_value(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_write_insert(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_read_cache(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyObject* mfile_read_editor_area(mfile_data_t* self, PyObject* dontuse)
{
    Py_RETURN_FALSE;
}

static PyMethodDef mfile_methods[] = {
    {"reload", (PyCFunction)mfile_reload, METH_NOARGS, ""},
    {"get_template", (PyCFunction)mfile_get_template, METH_NOARGS, ""},
    {"set_template", (PyCFunction)mfile_set_template, METH_NOARGS, ""},
    {"get_cursor", (PyCFunction)mfile_get_cursor, METH_NOARGS, ""},
    {"set_cursor", (PyCFunction)mfile_set_cursor, METH_NOARGS, ""},
    {"size", (PyCFunction)mfile_size, METH_NOARGS, ""},
    {"name", (PyCFunction)mfile_name, METH_NOARGS, ""},
    {"path", (PyCFunction)mfile_path, METH_NOARGS, ""},
    {"cache_init", (PyCFunction)mfile_cache_init, METH_NOARGS, ""},
    {"cache_loadzone", (PyCFunction)mfile_cache_loadzone, METH_NOARGS, ""},
    {"cache_reload", (PyCFunction)mfile_cache_reload, METH_NOARGS, ""},
    {"read", (PyCFunction)mfile_read, METH_NOARGS, ""},
    {"read_raw", (PyCFunction)mfile_read_raw, METH_NOARGS, ""},
    {"write", (PyCFunction)mfile_write, METH_NOARGS, ""},
    {"read_value", (PyCFunction)mfile_read_value, METH_NOARGS, ""},
    {"write_value", (PyCFunction)mfile_write_value, METH_NOARGS, ""},
    {"write_insert", (PyCFunction)mfile_write_insert, METH_NOARGS, ""},
    {"read_cache", (PyCFunction)mfile_read_cache, METH_NOARGS, ""},
    {"read_editor_area", (PyCFunction)mfile_read_editor_area, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyObject* mfile_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    mfile_data_t* self = NULL;

    /* Allocate space for the data struct */
    self = (mfile_data_t*)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }

    /* Set defaults */
    self->file = NULL;

    return (PyObject*)self;
}

static PyObject* mfile_dealloc(mfile_data_t* self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int mfile_init(mfile_data_t* self, PyObject* args, PyObject* kwds)
{
    return 0;
}

#define MFILE_DOC "It's a file!"

static PyTypeObject mfile_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mfile",                   /* tp_name */
    sizeof(mfile_data_t),      /* tp_basicsize */
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
    MFILE_DOC,                 /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    mfile_methods,             /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    mfile_new,                 /* tp_new */
};

static PyObject* mcore_file_exists(PyObject* self, PyObject* args)
{
    int rc = 0;
    const char* fname = NULL;

    /* Get arguments */
    if (!PyArg_ParseTuple(args, "s", &fname)) {
        return NULL;
    }

    printf("DEBUG: does %s exist? ", fname);

    rc = mc_file_access(fname);
    if (rc != 0) {
        Py_RETURN_FALSE;
    }

    Py_RETURN_TRUE;
}

static PyObject* mcore_file_open(PyObject* self, PyObject* args)
{
    printf("RAN A THING OHHMMEEEHGGEEEEEE");

    Py_RETURN_TRUE;
}

static PyObject* mcore_file_close(PyObject* self, PyObject* args)
{
    printf("RAN A THING OHHMMEEEHGGEEEEEE");

    Py_RETURN_TRUE;
}

static PyMethodDef mcore_file_methods[] = {
    {"exists", (PyCFunction)mcore_file_exists, METH_VARARGS, "Checks if a file exists."},
    {"open",   (PyCFunction)mcore_file_open,   METH_VARARGS, "Opens a file."},
    {"close",  (PyCFunction)mcore_file_close,  METH_VARARGS, "Closes a file."},
    {NULL, NULL, 0, NULL}
};

PyObject* create_mfile(mc_file_t* f)
{
    mfile_data_t* ret = NULL;

    /* Create object */
    ret = PyObject_New(mfile_data_t, &mfile_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Initialize object */
    ret = (mfile_data_t*)PyObject_Init((PyObject*)ret, &mfile_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Set file */
    ret->file = f;

    return (PyObject*)ret;
}

static PyModuleDef mcore_file_module = {
    PyModuleDef_HEAD_INIT,
    "mcore_file",
    "Bindings for mcore's file api.",
    -1,
    mcore_file_methods,
    NULL, NULL, NULL, NULL
};

static PyMODINIT_FUNC PyInit_mcore_file()
{
    PyObject* m = NULL;

    /* Assign new function */
    //mfile_type.tp_new = PyType_GenericNew;

    /* Make sure the type is ready to go */
    if (PyType_Ready(&mfile_type) < 0) {
        return NULL;
    }

    /* Create module */
    m = PyModule_Create(&mcore_file_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&mfile_type);
    PyModule_AddObject(m, "mfile", (PyObject*)&mfile_type);

    return m;
}

int init_binds_file()
{
    int rc = 0;

    rc = PyImport_AppendInittab("mcore_file", PyInit_mcore_file);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
