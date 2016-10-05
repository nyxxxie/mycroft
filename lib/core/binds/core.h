#ifndef BINDS_CORE_H
#define BINDS_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <mycroft/mycroft.h>
#include "core.h"

typedef struct {
    PyObject_HEAD
    mc_ctx_t* ctx;
} mcore_ctx_t;

PyObject* create_mcore_ctx(mc_ctx_t* c); 

int init_binds_core();

#ifdef __cplusplus
}
#endif

#endif // BINDS_CORE_H
