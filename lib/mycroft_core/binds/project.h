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
