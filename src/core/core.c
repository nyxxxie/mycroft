#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "core.h"
#include "config.h"
#include "database.h"
#include "file.h"

mc_ctx_t* mycroft_init() {

    /* Create the context */
    mc_ctx_t* ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (ctx == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    ctx->config = (mc_config_t*)malloc(sizeof(mc_config_t));
    if (ctx->config == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    ctx->db = (mc_mdb_t*)malloc(sizeof(mc_mdb_t));
    if (ctx->db == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    ctx->file = (mc_file_t*)malloc(sizeof(mc_file_t));
    if (ctx->file == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    return ctx;
}

void mycroft_free(mc_ctx_t* ctx) {
    if (ctx != NULL) {
        if (ctx->file != NULL) {
            file_close(ctx->file);
            free(ctx->file);
        }

        if (ctx->db != NULL) {
            mdb_close(ctx->db);
            free(ctx->db);
        }

        free(ctx);
    }
}

int mycroft_open_config(mc_ctx_t* ctx, const char* target_filename) {

}

int mycroft_open_file(mc_ctx_t* ctx, const char* target_filename) {

    /* Open target file */
    if (file_open(ctx->file, target_filename) < 0) {
        return -1;
    }

    /* Generate the mdb filename from the target's filename */
    int b = strlen(target_filename);
    char mdb_filename[b + 5];
    strcpy(mdb_filename, target_filename);
    mdb_filename[b]   = '.';
    mdb_filename[b+1] = 'm';
    mdb_filename[b+2] = 'd';
    mdb_filename[b+3] = 'b';
    mdb_filename[b+4] = '\0';

    /* Try to open/create the mdb file */
    mdb_init(ctx->db);
    if (file_exists(mdb_filename) == 0) {
        if (mdb_load_db(ctx->db, mdb_filename) < 0) {
            return -1;
        }
        if (mdb_set_project(ctx->db, ctx) < 0) {
            return -1;
        }
    } else {
        if (mdb_create_db(ctx->db, mdb_filename) < 0) {
            return -1;
        }
        if (mdb_load_project(ctx->db, ctx) < 0) {
            return -1;
        }
    }

    return 0;
}
