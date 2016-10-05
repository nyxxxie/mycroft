#include <mycroft/file.h>
#include "binds/file.h"

static PyObject* mfile_name(mfile_data_t* self, PyObject* dontuse) {
    
    if (self->file == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "object has no file");
        return NULL;
    }

    return PyUnicode_FromString(file_name(self->file));
}

static PyObject* mfile_path(mfile_data_t* self, PyObject* dontuse) {
 
    if (self->file == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "object has no file");
        return NULL;
    }

    return PyUnicode_FromString(file_path(self->file));
}

static PyObject* mfile_size(mfile_data_t* self, PyObject* dontuse) {
 
    if (self->file == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "object has no file");
        return NULL;
    }

    return PyLong_FromLongLong(file_size(self->file));
}

static PyMethodDef mfile_methods[] = {
    {"name", mfile_name, METH_NOARGS, ""},
    {"path", mfile_path, METH_NOARGS, ""},
    {"size", mfile_size, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyObject* mfile_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {

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

static PyObject* mfile_dealloc(mfile_data_t* self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int mfile_init(mfile_data_t* self, PyObject* args, PyObject* kwds) {
    return 0;
}

#define MFILE_DOC \
    "Object representing a file opened by mycroft.  This object must be "    \
    "used over the traditional file object offered by python because it is " \
    "associated with several internal constructs necessary to mycroft's "    \
    "operations."

static PyTypeObject mcore_file_mfile_type = {
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

static PyObject* mfile_exists(PyObject* self, PyObject* args) {

    int rc = 0;
    const char* fname = NULL;

    /* Get arguments */
    if (!PyArg_ParseTuple(args, "s", &fname)) {
        return NULL;
    }

    printf("DEBUG: does %s exist? ", fname);

    rc = file_exists(fname);
    if (rc < 0) {
        Py_RETURN_FALSE;
    }

    Py_RETURN_TRUE;
}

static PyObject* mfile_open(PyObject* self, PyObject* args) {

    printf("RAN A THING OHHMMEEEHGGEEEEEE");

    Py_RETURN_TRUE;
}

static PyObject* mfile_close(PyObject* self, PyObject* args) {

    printf("RAN A THING OHHMMEEEHGGEEEEEE");

    Py_RETURN_TRUE;
}

static PyMethodDef mcore_file_methods[] = {
    {"exists",    mfile_exists, METH_VARARGS, "Checks if a file exists."},
    {"open",      mfile_open,   METH_VARARGS, "Opens a file."},
    {"close",     mfile_close,  METH_VARARGS, "Closes a file."},
    {NULL, NULL, 0, NULL}
};

PyObject* create_mfile(mc_file_t* f) {

    mfile_data_t* ret = NULL;

    /* Create object */
    ret = PyObject_New(mfile_data_t, &mcore_file_mfile_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Initialize object */
    ret = PyObject_Init(ret, &mcore_file_mfile_type);
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

static PyMODINIT_FUNC PyInit_mcore_file() {

    PyObject* m = NULL;

    /* Assign new function */
    //mcore_file_mfile_type.tp_new = PyType_GenericNew;

    /* Make sure the type is ready to go */
    if (PyType_Ready(&mcore_file_mfile_type) < 0) {
        return NULL;
    }

    /* Create module */
    m = PyModule_Create(&mcore_file_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&mcore_file_mfile_type);
    PyModule_AddObject(m, "mfile", (PyObject*)&mcore_file_mfile_type);

    return m;
}

int init_binds_file() {

    int rc = 0;

    rc = PyImport_AppendInittab("mcore_file", PyInit_mcore_file);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
