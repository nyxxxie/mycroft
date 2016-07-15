#ifndef MYCROFT_CONFIG_H
#define MYCROFT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "core.h"

#define CMAKE_INSTALL_PREFIX ""

struct mc_config_t {
    int db_save_local;
    int db_strip_orig_ext;
};

static mc_config_t g_config;

/* Init/free */
void cfg_init(mc_config_t* ctx);
void cfg_close(mc_config_t* ctx);

void cfg_init_global();
void cfg_load_default_values(mc_config_t* config);
void cfg_load_file(mc_config_t* config, const char* custom_location);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_CONFIG_H
