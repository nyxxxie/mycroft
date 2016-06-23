#ifndef MYCROFT_H
#define MYCROFT_H

typedef struct mycroft_context_t mycroft_context_t;

mycroft_context_t* mycroft_init();
mycroft_context_t* mycroft_init_cfg(const char* config_file);
void mycroft_free(mycroft_context_t* ctx);

int mycroft_open_file(mycroft_context_t* ctx, const char* filename);

#endif // MYCROFT_H
