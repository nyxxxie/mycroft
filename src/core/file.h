#ifndef MYCROFT_INT_FILE_H
#define MYCROFT_INT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <mycroft/file.h>

/**
 * Represents the buffer used by the editor to display bytes.  We read a
 * little extra of the file in order to try and minimise disk access.
 */
typedef struct {
    uint8_t* buf;
    fsize_t  size;
    fsize_t  base;
    fsize_t  cursor; //TODO: change this as needed to work with the read/write editor area functions
} editor_buffer_t;

/** Contains info for an open file */
struct mc_file_t {
    char*   name;
    char*   path;
    FILE*   fp;
    fsize_t size;
    fsize_t cursor;

    editor_buffer_t buf;
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_FILE_H
