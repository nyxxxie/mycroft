#ifndef MYCROFT_ERROR_H
#define MYCROFT_ERROR_H

#include <stdio.h>

#define MC_ERROR(fmt, ...) \
    fprintf(stderr, "ERROR: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stderr, fmt, __VA_ARGS__);

#endif // MYCROFT_ERROR_H
