#ifndef MYCROFT_CONFIG_H
#define MYCROFT_CONFIG_H

#define CMAKE_INSTALL_PREFIX ""

typedef struct {
    int db_save_local;
    int db_strip_orig_ext;
} mycroft_config_values_t;

void load_config(mycroft_config_values_t* config, const char* custom_location);

// TODO: might need to add a cleanup function for string variables that we 
//       malloc?

#endif // MYCROFT_CONFIG_H
