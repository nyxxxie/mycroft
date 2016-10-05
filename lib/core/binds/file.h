#ifndef BINDS_FILE_H
#define BINDS_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>
#include <mycroft/file.h>

typedef struct {
    PyObject_HEAD
    mc_file_t* file;
} mfile_data_t;

PyObject* create_mfile(mc_file_t* f);

int init_binds_file();

#ifdef __cplusplus
}
#endif

#endif // BINDS_FILE_H
