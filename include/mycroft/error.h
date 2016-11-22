#ifndef MYCROFT_ERROR_H
#define MYCROFT_ERROR_H

#include <errno.h>

typedef enum {
    MC_ERR_INVALID = 0,
    MC_ERR, /** Generic error. */
    MC_OK
} mc_error_t;

// Optional arguments
#define __OARGS(...) , ##__VA_ARGS__

// Generic error output function
#define MC_ERROR(fmt, ...) \
    fprintf(stderr, "ERROR: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stderr, fmt __OARGS(__VA_ARGS__));

// Generic debug output function
#define MC_DEBUG(fmt, ...) \
    fprintf(stdout, "DEBUG: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stdout, fmt __OARGS(__VA_ARGS__));

#endif // MYCROFT_ERROR_H
