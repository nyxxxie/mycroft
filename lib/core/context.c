#include <stdio.h>
#include <unistd.h>
#include "context.h"
#include "database.h"

mc_ctx_t* mc_ctx_create() {
    mc_ctx_t* ctx = NULL;

    /* Allocate space for the ctx */
    ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }

    /* Set each field to default value */
    ctx->projects = NULL;
    ctx->project_amt = 0;
    ctx->project_focused = NULL;

    return ctx;
}

void mc_ctx_free(mc_ctx_t* ctx) {
    if (ctx != NULL) {
        if (ctx->projects != NULL) {
            mc_project_t* project = NULL;
            int i = 0;

            for (; i < ctx->project_amt; i++) {
                mc_project_free(ctx->projects[i]);
            }

            free(ctx->projects);
        }

        free(ctx);
    }
}

uint32_t mc_ctx_add_project(mc_ctx_t* ctx, mc_project_t* project) {
    int cur_index = 0;

    /* Save index that the project should be saved to, then iterate total */
    cur_index = ctx->project_amt;
    ctx->project_amt++;

    /* Create more space in the project array */
    ctx->projects = realloc(ctx->projects, ctx->project_amt);
    if (ctx->projects == NULL) {
        return (uint32_t)(-1);
    }

    /* Add project to the array */
    ctx->projects[cur_index] = project;

    return cur_index;
}

int mc_ctx_remove_project(mc_ctx_t* ctx, uint32_t project_index) {

    // TODO: implement

    return 0;
}

mc_project_t* mc_ctx_get_project(mc_ctx_t* ctx, uint32_t project_index) {

    if (ctx->projects) {
        return NULL;
    }

    return ctx->projects[project_index];
}

uint32_t mc_ctx_get_project_amount(mc_ctx_t* ctx) {
    return ctx->project_amt;
}

void mc_ctx_set_focused_project(mc_ctx_t* ctx, mc_project_t* project) {
    ctx->project_focused = project;
}

mc_project_t* mc_ctx_get_focused_project(mc_ctx_t* ctx) {
    return ctx->project_focused;
}

//mc_ctx_t* mycroft_init() {
//
//    int rc = 0;
//
//    /* Create the context */
//    mc_ctx_t* ctx = (mc_ctx_t*)malloc(sizeof(mc_ctx_t));
//    if (ctx == NULL) {
//        perror("Failed to allocate space for mc_ctx_t");
//        return NULL;
//    }
//
//    ctx->db = (mc_mdb_t*)malloc(sizeof(mc_mdb_t));
//    if (ctx->db == NULL) {
//        perror("Failed to allocate space for mc_mdb_t");
//        return NULL;
//    }
//    mdb_init(ctx->db);
//
//    ctx->file = (mc_file_t*)malloc(sizeof(mc_file_t));
//    if (ctx->file == NULL) {
//        perror("Failed to allocate space for mc_file_t");
//        return NULL;
//    }
//    file_init(ctx->file);
//
//    rc = mc_plugin_init();
//    if (rc < 0) {
//        fprintf(stderr, "Failed to initialize plugin system.");
//        return NULL;
//    }
//
//    return ctx;
//}
//
//void mycroft_free(mc_ctx_t* ctx) {
//
//    int rc = 0;
//
//    rc = mc_plugin_close();
//    if (rc < 0) {
//        fprintf(stderr, "Failed to close the plugin system.");
//    }
//
//    if (ctx != NULL) {
//        if (ctx->db != NULL) {
//            mdb_close(ctx->db);
//            free(ctx->db);
//        }
//        if (ctx->file != NULL) {
//            file_close(ctx->file);
//            free(ctx->file);
//        }
//
//        free(ctx);
//    }
//}
//
//int set_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {
//
//    char* tmp = NULL;
//
//    /* Set the file's name and size in the db. */
//    if (mdb_set_file_name(mdb, file_name(ctx->file))) {
//        return -1;
//    }
//    if (mdb_set_file_path(mdb, file_path(ctx->file))) {
//        return -1;
//    }
//    if (mdb_set_file_size(mdb, file_size(ctx->file))) {
//        return -1;
//    }
//
//    /* Hash the file and set the property in the table */
//    SHA256_CTX shactx;
//    SHA256_Init(&shactx);
//
//    char readbuf[1024];
//    while (1) {
//        int amnt = file_read(ctx->file, sizeof(readbuf), readbuf);
//        if (amnt == 0) {
//            break;
//        }
//        else if (amnt < 0) {
//            return -1;
//        }
//
//        SHA256_Update(&shactx, readbuf, amnt);
//    }
//
//    mdb_hash_t hash;
//    SHA256_Final(hash.bytes, &shactx);
//
//    if (mdb_set_file_hash(mdb, &hash)) {
//        return -1;
//    }
//
//    file_set_cursor(ctx->file, 0);
//
//    return 0;
//}
//
//int load_project(mc_mdb_t* mdb, mc_ctx_t* ctx) {
//
//}
//
//int mycroft_open_file(mc_ctx_t* ctx, const char* target_filename) {
//
//    int b = strlen(target_filename);
//    char mdb_filename[b + 5];
//
//    /* Open target file */
//    if (file_open(ctx->file, target_filename) < 0) {
//        printf("Failed to open file.\n");
//        return -1;
//    }
//
//    /* Generate the mdb filename from the target's filename */
//    strcpy(mdb_filename, target_filename);
//    mdb_filename[b]   = '.';
//    mdb_filename[b+1] = 'm';
//    mdb_filename[b+2] = 'd';
//    mdb_filename[b+3] = 'b';
//    mdb_filename[b+4] = '\0';
//
//    /* Try to open/create the mdb file */
//    mdb_init(ctx->db);
//    if (file_exists(mdb_filename) == 0) {
//        if (mdb_load_db(ctx->db, mdb_filename) < 0) {
//            return -1;
//        }
//        if (set_project(ctx->db, ctx) < 0) {
//            return -1;
//        }
//    } else {
//        if (mdb_create_db(ctx->db, mdb_filename) < 0) {
//            return -1;
//        }
//        if (load_project(ctx->db, ctx) < 0) {
//            return -1;
//        }
//    }
//
//    return 0;
//}
//
//mc_file_t* mycroft_get_file(mc_ctx_t* ctx) {
//    return ctx->file;
//}
//
//mc_mdb_t*  mycroft_get_mdb(mc_ctx_t* ctx) {
//    return ctx->db;
//}
