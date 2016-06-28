#ifndef MYCROFT_CONFIG_H
#define MYCROFT_CONFIG_H

#define CMAKE_INSTALL_PREFIX ""

typedef struct {
    int db_save_local;
    int db_strip_orig_ext;
} mc_config_t;
mc_config_t g_config;

void cfg_init_global();
void cfg_load_default_values(mc_config_t* config);
void cfg_load_file(mc_config_t* config, const char* custom_location);

// TODO: might need to add a cleanup function for string variables that we 
//       malloc?

#endif // MYCROFT_CONFIG_H
