#ifndef MYCROFT_SCRIPT_H
#define MYCROFT_SCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/error.h>

mc_error_t mc_script_runfile(const char* path);
mc_error_t mc_script_runstring(const char* script);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_SCRIPT_H
