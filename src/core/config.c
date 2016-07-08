#include <stdio.h>
#include "config.h"
#include "config_parse.h"

void load_default_values(mc_config_t* config) {
    config->db_save_local     = 0;
    config->db_strip_orig_ext = 0;
}

void cfg_init(mc_config_t* ctx) {
    load_default_values(ctx);
}

void cfg_close(mc_config_t* ctx) {

}

void cfg_load_file(mc_config_t* config, const char* custom_location) {

    //TODO: change this to make it more neat

    FILE* fp = NULL;

    /* Try to load config values from a config file. */
    if (custom_location != NULL) {
        fp = fopen(custom_location, "r");
    }
    if (fp == NULL) {
        //TODO: add in a line in the cmake config that sets the home dir config folder
        fp = fopen("~/.config/mycroft.conf", "r");
        if (fp == NULL) {
            fp = fopen(CMAKE_INSTALL_PREFIX "/share/config/mycroft.conf", "r");
        }
    }
    printf(CMAKE_INSTALL_PREFIX "/share/config/mycroft.conf");

    /* Either load the config from file or set default values */
    if (fp != NULL) {
        printf("DEBUG: Using config file.\n");
        config_parse(fp, config);
        fclose(fp);
    }
    else {
        printf("DEBUG: Couldn't find a config file to use, using default values.\n");
        load_default_values(config);
    }
}

void cfg_init_global() {
    cfg_load_file(&g_config, NULL);
}

