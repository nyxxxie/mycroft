#ifndef MYCROFT_FILE_H
#define MYCROFT_FILE_H

#include <stdint.h>
#include <stdio.h>
#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Size used by internal file functions */
typedef long int fsize_t;

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

/* Util functions */
int file_exists(const char* file);

/* Init/free functions */
int file_init(mc_file_t* file);
int file_open(mc_file_t* file, const char* file_name);
int file_close(mc_file_t* file);

/* Management functions */
int file_get_cursor(mc_file_t* file);
int file_set_cursor(mc_file_t* file, int cursor);
fsize_t file_size(mc_file_t* file);
char* file_name(mc_file_t* file);
char* file_path(mc_file_t* file);

/* Read/write functions */
int file_read(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int file_read_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_read_editor_area(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_write(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int file_write_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_write_editor_area(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_write_insert(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_FILE_H
