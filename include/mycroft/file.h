#ifndef MYCROFT_FILE_H
#define MYCROFT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** Size used by internal file functions */
typedef long int fsize_t;

typedef struct mc_file_t mc_file_t;

/* Util functions */
int file_exists(const char* file);

/* Init/free functions */
int file_init(mc_file_t* file);
int file_open(mc_file_t* file, const char* file_name);
int file_close(mc_file_t* file);
int file_reload(mc_file_t* file);

/* Management functions */
int file_get_cursor(mc_file_t* file);
int file_set_cursor(mc_file_t* file, int cursor);
fsize_t file_size(mc_file_t* file);
char* file_name(mc_file_t* file);
char* file_path(mc_file_t* file);

/* Cache functions */
int file_cache_init(mc_file_t* file, int size);
int file_cache_loadzone(mc_file_t* file, int cursor);
int file_cache_reload(mc_file_t* file);

/* Read/write functions */
int file_read(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int file_read_raw(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int file_write(mc_file_t* file, fsize_t amount, uint8_t* outbuf);
int file_read_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_write_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_write_insert(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_read_cache(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);
int file_read_editor_area(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_FILE_H
