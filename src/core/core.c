#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

#include "core.h"
#include "config.h"

mc_ctx_t* mycroft_init() {
    return mycroft_init_cfg("\0");
}

mc_ctx_t* mycroft_init_cfg(const char* config_filename) {

    /* Create the context */
    mc_ctx_t* ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (ctx == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    /* Quick and dirty way to init all values to 0 */
    memset(ctx, sizeof(mc_ctx_t), 0);

    /* Load config */
    load_config(&g_config, config_filename);
    ctx->config = g_config;     //TODO: maybe just make config global?
    return ctx;
}

void mycroft_free(mc_ctx_t* state) {
    file_close(&state->file);
    mdb_close(&state->db);
    free(state);
}

//int mycroft_open_mdb(mc_ctx_t* ctx, const char* mdb_file) {
//
//    /* Check if mdb file exists */
//    if (file_exists(mdb_file) == 0) {
//        printf("DEBUG: Couldn't find  mdb file \"%s\"\n", mdb_file);
//        return -1;
//    }
//
//    if (mdb_load_mdb(&ctx->db, mdb_file) < 0) {
//        return -1;
//    }
//
//    return 0;
//}

int mycroft_open_file(mc_ctx_t* ctx, const char* target_filename) {

    //TODO: this could probably be neater :/
    /* Open target file */
    if (file_open(&ctx->file, target_filename) < 0) {
        return -1;
    }

    /* Open mdb file */
    mdb_init(&ctx->db);
    if (mdb_load_target(&ctx->db, &ctx->file)) {
        return -1;
    }
}
