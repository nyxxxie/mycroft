#ifndef MYCROFT_FILE_H
#define MYCROFT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mycroft/template.h>
#include <mycroft/error.h>

/** Size used by internal file functions */
typedef long int fsize_t;

typedef struct mc_file_t mc_file_t;

/* Util functions */
int file_exists(const char* file);

/* Init/free functions */
mc_file_t* mc_file_open(const char* file_name);
int mc_file_close(mc_file_t* file);
int mc_file_reload(mc_file_t* file);

/* Template functions */
template_t* mc_file_get_template(mc_file_t* file);
int mc_file_set_template(mc_file_t* file, template_t* t);

/* Management functions */
int mc_file_get_cursor(mc_file_t* file);
int mc_file_set_cursor(mc_file_t* file, int cursor);
fsize_t mc_file_size(mc_file_t* file);
char* mc_file_name(mc_file_t* file);
char* mc_file_path(mc_file_t* file);

/* Cache functions */
int mc_file_cache_init(mc_file_t* file, int size);
int mc_file_cache_loadzone(mc_file_t* file, int cursor);
int mc_file_cache_reload(mc_file_t* file);

/* Read/write functions */
int mc_file_read(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int mc_file_read_raw(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int mc_file_write(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int mc_file_read_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int mc_file_write_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int mc_file_write_insert(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int mc_file_read_cache(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int mc_file_read_editor_area(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);

/* Misc functions */
mc_error_t mc_file_hash(mc_file_t* file, uint8_t** hash, size_t* hashlen);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_FILE_H
