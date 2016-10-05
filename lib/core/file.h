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
    uint8_t* buf;   /**< @internal */
    fsize_t  size;  /**< @internal */
    fsize_t  base;  /**< @internal */
} file_cache_t;

/**
 * Contains state information for an open file.  Don't try to use any of these
 * fields, they may change in the future.
 */
struct mc_file_t {
    char*   name;   /**< @internal */
    char*   path;   /**< @internal */
    FILE*   fp;     /**< @internal */
    fsize_t size;   /**< @internal */
    fsize_t cursor; /**< @internal */
    template_t* t;  /**< @internal */
    file_cache_t* cache; /**< @internal */
};

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_INT_FILE_H
