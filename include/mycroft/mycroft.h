#ifndef MYCROFT_H
#define MYCROFT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mc_ctx_t mc_ctx_t;

/* Init/free */
mc_ctx_t* mycroft_init();
void mycroft_free(mc_ctx_t* ctx);

int mycroft_open_file(mc_ctx_t* ctx, const char* filename);
int mycroft_open_mdb(mc_ctx_t* ctx, const char* filename);
int mycroft_open_config(mc_ctx_t* ctx, const char* filename);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_H
