/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "template_parser.h"

#define PERR_UNEXP_CHAR(_CTX_, _CHAR_)                                   \
        fprintf(stderr,                                                  \
            "Unexpected char '%c' (0x%02x) encountered while parsing.\n" \
            "\tPARSER:  %s\n"                                            \
            "\tLINE NO: %i\n"                                            \
            "\tBYTE NO: %i\n",                                           \
            _CHAR_, _CHAR_, __func__, _CTX_->line, _CTX_->byte);

#define PERR_UNEXP_EOF(_CTX_)                             \
        fprintf(stderr,                                   \
            "Unexpected EOF encountered while parsing.\n" \
            "\tPARSER:  %s\n"                             \
            "\tLINE NO: %i\n"                             \
            "\tBYTE NO: %i\n",                            \
            __func__, _CTX_->line, _CTX_->byte);

void wcap_ctx_init(wcapture_ctx_t* ctx) {
    ctx->state = WCSTATE_IDLE;
}

void wcap_ctx_free(wcapture_ctx_t* ctx) {
    if (ctx->word.val != NULL) {
        free(ctx->word.val);
    }
}

void parse_ctx_init(parse_ctx_t* ctx) {
    ctx->byte = 0;
    ctx->line = 0;
    ctx->templt = NULL;
    wcap_ctx_init(&ctx->wcap);
    ctx->state.state      = PSTATE_BODY;
    ctx->state.member     = NULL;
    ctx->state.struct_def = NULL;
}

void parse_ctx_free(parse_ctx_t* ctx) {
    printf("TODO: implement %s.\n", __func__);
}

int iswordchar(int c) {
    return (isalpha(c) | isdigit(c) | (c == '_') | (c == '-'));
}

int parse_word_pre(parse_ctx_t* ctx, FILE* fp, int c) {

    int rc = 0;
    const int state = ctx->wcap.state; // to cut down on clutter in code

    /* Determine if this is a character that can be used in a word */
    if (iswordchar(c)) {
        if (state == WCSTATE_IDLE) {
            if (isalpha(c)) {
                ctx->wcap.state = WCSTATE_INWORD;
                ctx->wcap.word.start = ctx->byte;
                ctx->wcap.word.size = 1;
            }
        }
        else if (state == WCSTATE_INWORD) {
            ctx->wcap.word.size++;
        }
    }
    else {
        if (state == WCSTATE_INWORD) {

            /* Allocate space for word */
            ctx->wcap.word.val = (char*)malloc(ctx->wcap.word.size+1);
            if (ctx->wcap.word.val == NULL) {
                fprintf(stderr, "Failed to alloc space for word.\n");
                return -1;
            }

            /* Read word */
            fseek(fp, ctx->wcap.word.start, SEEK_SET); // go to where word starts
            fread(ctx->wcap.word.val, 1, ctx->wcap.word.size, fp);
            fseek(fp, ftell(fp)+1, SEEK_SET);   // +1 file cursor

            /* Append null terminator to word */
            ctx->wcap.word.val[ctx->wcap.word.size] = '\0';

            /* Update state */
            ctx->wcap.state = WCSTATE_DONE;
        }
    }

    return rc;
}

int parse_word_post(parse_ctx_t* ctx, int c) {

    if (ctx->wcap.state == WCSTATE_DONE) {
        if (ctx->wcap.word.val != NULL) {
            free(ctx->wcap.word.val);
        }
        ctx->wcap.state = WCSTATE_IDLE;
    }

    return 0;
}

int parse_body(parse_ctx_t* ctx, int c) {

    if (c == '/') {
        ctx->state.state = PSTATE_COMMENT_START;
    }
    else if (ctx->wcap.state == WCSTATE_DONE) {
        if (memcmp(ctx->wcap.word.val, "struct", ctx->wcap.word.size) == 0) {
            ctx->state.state = PSTATE_STRUCT_NAME;
        }
    }
    else if (iswordchar(c) || isspace(c) || (c == ';') || (c == EOF)) {
        return 0; // Don't care!
    }
    else {
        fprintf(stderr, "Unexpected char '%c' (0x%02x) while parsing body.\n", c, c);
        return -1;
    }

    return 0;
}

int parse_comment_start(parse_ctx_t* ctx, int c) {

    if (c == '*') {
        ctx->state.state = PSTATE_COMMENT_MLINE;
    }
    else if (c == '/') {
        ctx->state.state = PSTATE_COMMENT_SLINE;
    }
    else {
        fprintf(stderr, "Unexpected char '%c' (0x%02x) while determining comment type.\n", c, c);
        return -1;
    }

    return 0;
}

int parse_comment_mline(parse_ctx_t* ctx, int c) {

    /* Check if we hit EOF */
    if (c == EOF) {
        fprintf(stderr, "Unexpected EOF in multiline comment.\n");
        return -1;
    }
    else if (c == '*') {
        ctx->state.state = PSTATE_COMMENT_MLINE_AST;
    }

    return 0;
}

int parse_comment_mline_ast(parse_ctx_t* ctx, int c) {

    if (c == EOF) {
        fprintf(stderr, "Unexpected EOF in multiline comment.\n");
        return -1;
    }
    else if (c == '/') {
        ctx->state.state = PSTATE_BODY;
    }

    return 0;
}

int parse_comment_sline(parse_ctx_t* ctx, int c) {

    if (c == '\n' || c == '\r') {
        ctx->state.state = PSTATE_BODY;
    }

    return 0;
}

int parse_struct_name(parse_ctx_t* ctx, int c) {

    if (c == EOF) {
        PERR_UNEXP_EOF(ctx);
        return -1;
    }
    else if (c == ';') {
        PERR_UNEXP_CHAR(ctx, c);
        return -1;
    }
    else if (ctx->wcap.state == WCSTATE_DONE) {
        struct_def_t* strct = struct_def_create();
        if (strct == NULL) {
            return -1;
        }

        /* Hand over control of the name mem to our struct */
        strct->name = ctx->wcap.word.val;
        ctx->wcap.word.val = NULL;

        /* Make parser aware of our struct */
        ctx->state.struct_def = strct;

        ctx->state.state = PSTATE_STRUCT_BODY;
    }

    return 0;
}

int parse_struct_body(parse_ctx_t* ctx, int c) {

    int rc = 0;

    if (iswordchar(c)) {
        ctx->state.state = PSTATE_STRUCT_FIELD_TYPE;
        return parse_struct_field_type(ctx, c);
    }
    else if (c == '{') {
        ctx->state.state = PSTATE_STRUCT_FIELD_TYPE;
    }
    else if (c == '}') {
        rc = template_add_struct_def(ctx->templt, ctx->state.struct_def);
        if (rc < 0) {
            return rc;
        }
        ctx->state.member = NULL;
        ctx->state.state  = PSTATE_BODY;
    }

    return 0;
}

int parse_struct_field_type(parse_ctx_t* ctx, int c) {

    if (ctx->wcap.state == WCSTATE_DONE) {

        /* Create field */
        member_t* member = member_create();
        if (member == NULL) {
            return -1;
        }

        /* Hand over control of the word mem to our type */
        member->type = ctx->wcap.word.val;
        ctx->wcap.word.val = NULL;

        /* Make parser aware of our field */
        ctx->state.member = member;

        ctx->state.state = PSTATE_STRUCT_FIELD_NAME;
    }

    return 0;
}

int parse_struct_field_name(parse_ctx_t* ctx, int c) {

    int rc = 0;

    if (ctx->wcap.state == WCSTATE_DONE) {
        if (c == ';') {

            /* Hand over control of the word mem to our type */
            ctx->state.member->name = ctx->wcap.word.val;
            ctx->wcap.word.val = NULL;

            /* Add current field to struct */
            rc = struct_def_add_member(ctx->state.struct_def, ctx->state.member);
            if (rc < 0) {
                return rc;
            }

            ctx->state.state = PSTATE_STRUCT_BODY;
        }
        else if (c == '[') {
            fprintf(stderr, "Array types not implemented (but they will be!)\n");
            return -1;
        }
        else {
            PERR_UNEXP_CHAR(ctx, c);
            return -1;
        }
    }

    return 0;
}

int parse_operator(parse_ctx_t* ctx, int c) {
    return 0;
}

int parse_operator_assign(parse_ctx_t* ctx, int c) {
    return 0;
}

int parse_var_decl(parse_ctx_t* ctx, int c) {
    return 0;
}

int parse_template_file(template_t* t, const char* file) {

    FILE* fp = NULL;
    int rc   = 0;
    parse_ctx_t ctx;

    /* Init parser ctx */
    parse_ctx_init(&ctx);
    ctx.templt = t;

    /* Open file */
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Couldn't find file \"%s\"\n", file);
        return -1;
    }

    /* Parse file one char at a time */
    while (1) {
        int c = 0;

        /* Read character */
        c = fgetc(fp);

        /* Process word */
        rc = parse_word_pre(&ctx, fp, c);
        if (rc < 0) {
            break;
        }

        /* */
        rc = (*parser_funcs[ctx.state.state])(&ctx, c);
        if (rc < 0) {
            break;
        }

        rc = parse_word_post(&ctx, c);
        if (rc < 0) {
            break;
        }

        /* Up the various counters */
        ctx.byte++;
        if (c == '\n') {
            ctx.line++;
        }

        /* Handle any parsing errors */
        if (rc < 0) {
            break;
        }

        /* Handle EOF */
        if (c == EOF) {
            fclose(fp); // Done with the template file
            return 0;
        }
    }

    fclose(fp);
    return -1;
}
