#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include "core.h"
#include "config.h"

mycroft_context_t* mycroft_init() {
    return mycroft_init_cfg("\0");
}

mycroft_context_t* mycroft_init_cfg(const char* config_file) {

    /* Create the context */
    mycroft_context_t* ctx = (mycroft_context_t*)malloc(sizeof(mycroft_context_t));
    if (ctx == NULL) {
        perror("Failed to allocate space for mycroft_context_t");
        return NULL;
    }

    /* Quick and dirty way to init all values to 0 */
    memset(ctx, sizeof(mycroft_context_t), 0);

    /* Load config */
    load_config(&ctx->config, config_file);

    return ctx;
}

void mycroft_free(mycroft_context_t* state) {
    file_close(&state->file);
    db_close(&state->db);
    free(state);
}

int mycroft_open_mdb(mycroft_context_t* ctx, const char* mdb_file) {

    /* Check if mdb file exists */
    if (file_exists(mdb_file) == 0) {
        printf("DEBUG: Couldn't find  mdb file \"%s\"\n", mdb_file);
        return -1;
    }

    if (db_load_from_file(&ctx->db, mdb_file) < 0) {
        return -1;
    }

    return 0;
}

int mycroft_open_file(mycroft_context_t* ctx, const char* target_file) {

    //TODO: this could probably be neater :/

    /* Check if target file exists */
    if (file_exists(target_file) < 0) {
        printf("DEBUG: Couldn't find  target file \"%s\".\n", target_file);
        return -1;
    }

    /* Open target file */
    if (file_open(&ctx->file, target_file) < 0) {
        return -1;
    }

    /* Save the db locally or at the file's location? */
    char* name;
    if (ctx->config.db_save_local) {
        printf("DEBUG: using local db\n");
        name = basename(target_file);
    }
    else {
        printf("DEBUG: using normal db\n");
        name = target_file;
    }

    /* Copy file path to our db path */
    char* mdb_filename = (char*)malloc(strlen(name) + 5);
    if (mdb_filename == NULL) {
        fprintf(stderr, "Failed to allocate space for mdb_filename");
        return -1;
    }
    strncpy(mdb_filename, target_file, strlen(name));

    /* Strip the file extension if we need to */
    if (ctx->config.db_strip_orig_ext) {
        //TODO: I don't think we should actually make this an option
        char* lastslash = strrchr(mdb_filename, '/');
        char* lastdot = strrchr(mdb_filename, '.');
        if (lastslash < lastdot) {
            mdb_filename[lastdot-mdb_filename] = '\0';
        }
    }

    /* Append the database extension */
    int b = strlen(mdb_filename);
    mdb_filename[b]   = '.';
    mdb_filename[b+1] = 'm';
    mdb_filename[b+2] = 'd';
    mdb_filename[b+3] = 'b';
    mdb_filename[b+4] = '\0';

    printf("target = %s\ndbfile = %s\n", target_file, mdb_filename);

    /* Attempt to open/create database */
    if (file_exists(mdb_filename) < 0) {
        printf("DEBUG: Creating db file.\n");
        if (db_create_file(&ctx->db, mdb_filename)) {
            return -1;
        }
    }
    else {

        //TODO: call db_set_target_file inside of db_load_from_file,
        //      modify db_load_from_file to take a file struct thing
        //      instead of a filename
        printf("DEBUG: Using existing db file.\n");
        if (db_load_from_file(&ctx->db, mdb_filename)) {
            return -1;
        }

        /* Inform the project what file we're working with */
        if (db_set_target_file(&ctx->db, &ctx->file)) {
            return -1;
        }
    }

    free(mdb_filename);
}
