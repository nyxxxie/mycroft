#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "core.h"
#include "cfg.h"
#include "database.h"
#include "file.h"
#include "plugin.h"

mc_ctx_t* mycroft_init() {

    int rc = 0;

    /* Create the context */
    mc_ctx_t* ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (ctx == NULL) {
        perror("Failed to allocate space for mc_ctx_t");
        return NULL;
    }

    ctx->config = (mc_config_t*)malloc(sizeof(mc_config_t));
    if (ctx->config == NULL) {
        perror("Failed to allocate space for mc_config_t");
        return NULL;
    }
    cfg_init(ctx->config);

    ctx->db = (mc_mdb_t*)malloc(sizeof(mc_mdb_t));
    if (ctx->db == NULL) {
        perror("Failed to allocate space for mc_mdb_t");
        return NULL;
    }
    mdb_init(ctx->db);

    ctx->file = (mc_file_t*)malloc(sizeof(mc_file_t));
    if (ctx->file == NULL) {
        perror("Failed to allocate space for mc_file_t");
        return NULL;
    }
    file_init(ctx->file);

    rc = mc_plugin_init();
    if (rc < 0) {
        fprintf(stderr, "Failed to initialize plugin system.");
        return NULL;
    }

    return ctx;
}

void mycroft_free(mc_ctx_t* ctx) {

    int rc = 0;

    rc = mc_plugin_close();
    if (rc < 0) {
        fprintf(stderr, "Failed to close the plugin system.");
        return NULL;
    }

    if (ctx != NULL) {
        if (ctx->config != NULL) {
            cfg_close(ctx->config);
            free(ctx->config);
        }
        if (ctx->db != NULL) {
            mdb_close(ctx->db);
            free(ctx->db);
        }
        if (ctx->file != NULL) {
            file_close(ctx->file);
            free(ctx->file);
        }

        free(ctx);
    }
}

int mycroft_open_config(mc_ctx_t* ctx, const char* target_filename) {

}

int set_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {

    char* tmp = NULL;

    /* Set the file's name and size in the db. */
    if (mdb_set_file_name(mdb, file_name(ctx->file))) {
        return -1;
    }
    if (mdb_set_file_path(mdb, file_path(ctx->file))) {
        return -1;
    }
    if (mdb_set_file_size(mdb, file_size(ctx->file))) {
        return -1;
    }

    /* Hash the file and set the property in the table */
    SHA256_CTX shactx;
    SHA256_Init(&shactx);

    char readbuf[1024];
    while (1) {
        int amnt = file_read(ctx->file, sizeof(readbuf), readbuf);
        if (amnt == 0) {
            break;
        }
        else if (amnt < 0) {
            return -1;
        }

        SHA256_Update(&shactx, readbuf, amnt);
    }

    mdb_hash_t hash;
    SHA256_Final(hash.bytes, &shactx);

    if (mdb_set_file_hash(mdb, &hash)) {
        return -1;
    }

    file_set_cursor(ctx->file, 0);

    return 0;
}

int load_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {

}


int mycroft_open_file(mc_ctx_t* ctx, const char* target_filename) {

    int b = strlen(target_filename);
    char mdb_filename[b + 5];

    /* Open target file */
    printf("Opening %s\n", target_filename);
    if (file_open(ctx->file, target_filename) < 0) {
        printf("NOOOO\n");
        return -1;
    }
    printf("YEEE\n");

    /* Generate the mdb filename from the target's filename */
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
        if (set_project(ctx->db, ctx) < 0) {
            return -1;
        }
    } else {
        if (mdb_create_db(ctx->db, mdb_filename) < 0) {
            return -1;
        }
        if (load_project(ctx->db, ctx) < 0) {
            return -1;
        }
    }

    return 0;
}

mc_file_t* mycroft_get_file(mc_ctx_t* ctx) {
    return ctx->file;
}

mc_mdb_t*  mycroft_get_mdb(mc_ctx_t* ctx) {
    return ctx->db;
}
