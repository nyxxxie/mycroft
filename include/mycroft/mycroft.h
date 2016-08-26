#ifndef MYCROFT_H
#define MYCROFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mycroft/mdb.h>
#include <mycroft/file.h>

typedef struct mc_ctx_t mc_ctx_t;
typedef struct mc_mdb_t mc_mdb_t;

mc_ctx_t* mycroft_get_ctx();

/* Init/free */
mc_ctx_t* mycroft_init();
void mycroft_free(mc_ctx_t* ctx);

int mycroft_open_file(mc_ctx_t* ctx, const char* filename);
int mycroft_open_mdb(mc_ctx_t* ctx, const char* filename);
int mycroft_open_config(mc_ctx_t* ctx, const char* filename);

mc_file_t* mycroft_get_file(mc_ctx_t* ctx);
mc_mdb_t*  mycroft_get_mdb(mc_ctx_t* ctx);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_H
