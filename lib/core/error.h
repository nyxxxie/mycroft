#ifndef MYCROFT_INT_ERROR_H
#define MYCROFT_INT_ERROR_H

#include <mycroft/error.h>
#include <stdio.h>

// Optional arguments
#define __OARGS(...) , ##__VA_ARGS__

// Generic error output function
#define MC_ERROR(fmt, ...) \
    fprintf(stderr, "ERROR: %s:%i - ",  __FILE__, __LINE__); \
    fprintf(stderr, fmt __OARGS(__VA_ARGS__));

#endif // MYCROFT_INT_ERROR_H
