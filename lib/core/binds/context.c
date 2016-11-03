#include "binds/context.h"
#include "binds/file.h"
#include "context.h"

static PyObject* mctx_get_project_amt(mcore_ctx_t* self, PyObject* dontuse) {

}

static mfile_data_t* mctx_get_project(mcore_ctx_t* self, PyObject* dontuse) {

}

static PyObject* mctx_projects(mcore_ctx_t* self, PyObject* dontuse) {

}

static PyMethodDef mcore_ctx_methods[] = {
    {"get_project_amount", (PyCFunction)mctx_get_project_amt, METH_NOARGS, ""},
    {"get_project", (PyCFunction)mctx_get_project, METH_NOARGS, ""},
    {"projects", (PyCFunction)mctx_projects, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static mcore_ctx_t* mcore_ctx_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    mcore_ctx_t* self = NULL;

    /* Allocate space for the data struct */
    self = (mcore_ctx_t*)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }

    /* Set defaults */
    printf("mcore_ctx_new NOT IMPLEMENTED OH GOD\n");
    self->ctx = NULL;

    return self;
}

static PyObject* mcore_ctx_dealloc(mfile_data_t* self) {

}

static int mcore_ctx_init(mfile_data_t* self, PyObject* args, PyObject* kwds) {
    return 0;
}

#define MCORE_CTX_DOC \
    "Represents an opened instance of mycroft."

static PyTypeObject mcore_ctx_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mcore.ctx",               /* tp_name */
    sizeof(mcore_ctx_t),       /* tp_basicsize */
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
    MCORE_CTX_DOC,             /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    mcore_ctx_methods,         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    mcore_ctx_new,             /* tp_new */
};

//static mcore_ctx_t* mcore_get_ctx(mcore_ctx_t* self, PyObject* dontuse) {
//
//    mcore_ctx_t* ctx = NULL;
//
//    /* Create ctx */
//    ctx = create_mcore_ctx(mycroft_get_ctx());
//    if (ctx == NULL) {
//        PyErr_SetString(PyExc_RuntimeError, "failed to create mctx object");
//        return NULL;
//    }
//
//    return ctx;
//}

static PyMethodDef mcore_methods[] = {
    {NULL, NULL, 0, NULL}
};

PyObject* create_mcore_ctx(mc_ctx_t* c) {

    mcore_ctx_t* ret = NULL;

    /* Create object */
    ret = PyObject_New(mcore_ctx_t, &mcore_ctx_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Initialize object */
    ret = PyObject_Init(ret, &mcore_ctx_type);
    if (ret == NULL) {
        return NULL;
    }

    /* Set ctx */
    ret->ctx = c;

    return (PyObject*)ret;
}

static PyModuleDef mcore_module = {
    PyModuleDef_HEAD_INIT,
    "mcore_ctx",
    "Bindings for mcore's file api.",
    -1,
    mcore_methods,
    NULL, NULL, NULL, NULL
};

static PyMODINIT_FUNC PyInit_mcore() {
    PyObject* m = NULL;

    /* Assign new function */
    mcore_ctx_type.tp_new = PyType_GenericNew;

    /* Make sure the type is ready to go */
    if (PyType_Ready(&mcore_ctx_type) < 0) {
        return NULL;
    }

    /* Create module */
    m = PyModule_Create(&mcore_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&mcore_ctx_type);
    PyModule_AddObject(m, "mfile", (PyObject*)&mcore_ctx_type);

    return m;
}


int init_binds_core() {
    int rc = 0;

    rc = PyImport_AppendInittab("mcore", PyInit_mcore);
    if (rc < 0) {
        return -1;
    }

    return 0;
}
