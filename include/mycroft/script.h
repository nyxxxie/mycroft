#ifndef MYCROFT_SCRIPT_H
#define MYCROFT_SCRIPT_H

#include <mycroft/error.h>

mc_error_t mc_script_runfile(const char* path);
mc_error_t mc_script_runstring(const char* script);

#endif // MYCROFT_SCRIPT_H
