#ifndef MYCROFT_H
#define MYCROFT_H

typedef struct mc_ctx_t mc_ctx_t;

mc_ctx_t* mycroft_init();
mc_ctx_t* mycroft_init_cfg(const char* config_file);
void mycroft_free(mc_ctx_t* ctx);

int mycroft_open_file(mc_ctx_t* ctx, const char* filename);

#endif // MYCROFT_H
