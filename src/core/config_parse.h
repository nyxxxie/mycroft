#ifndef MYCROFT_CONFIG_PARSE_H
#define MYCROFT_CONFIG_PARSE_H

#include <stdio.h>
#include "config.h"

int config_parse(FILE* fp, mc_config_t* config);

#endif // MYCROFT_CONFIG_PARSE_H