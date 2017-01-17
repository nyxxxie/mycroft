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

#ifndef TEMPLATE_PARSER_H
#define TEMPLATE_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <mycroft/template.h>

/**
 * This struct starts at the top of the templated file.
 */
#define TPARSER_ENTRY "FILE"

typedef enum {
    WCSTATE_IDLE = 0, /** Capturer isn't in a word right now */
    WCSTATE_INWORD,   /** Capturer is in a word */
    WCSTATE_DONE      /** Capturer has completed parsing a word */
} wcstate_t;

typedef struct {
    int   start;
    int   size;
    char* val;
} wcap_word_t;

typedef struct {
    wcap_word_t word;  /** When the word parser finds a word, this is that word */
    wcstate_t   state; /** Word parser state */
} wcapture_ctx_t;

void wcap_ctx_init(wcapture_ctx_t* ctx);
void wcap_ctx_free(wcapture_ctx_t* ctx);

typedef enum {
    PSTATE_BODY = 0,          /** Parser is in main body of file */
    PSTATE_COMMENT_START,     /** Parser has encountered the beginning of a comment */
    PSTATE_COMMENT_MLINE,     /** Parser is in a multiline comment */
    PSTATE_COMMENT_MLINE_AST, /** Parser encountered an astrisk (potential end?) */
    PSTATE_COMMENT_SLINE,     /** Parser is in a single line */
    PSTATE_STRUCT_NAME,       /** Parser is parsing a struct name */
    PSTATE_STRUCT_BODY,       /** Parser is parsing a struct body */
    PSTATE_STRUCT_FIELD_TYPE, /** Parser is parsing a struct field type */
    PSTATE_STRUCT_FIELD_NAME, /** Parser is parsing a struct fielf name */
    PSTATE_OPERATOR,          /** Parser is parsing for an operator */
    PSTATE_OPERATOR_ASSIGN,   /** Parser is parsing an assignment operator */
    PSTATE_VAR_DECL,          /** Parser is parsing a variable declaration */
    PSTATE_TOTAL,             /** Total number of states the parser can be in */
} pstate_t;


//TODO: actually use this like a stack
typedef struct state_stack_ent_t state_stack_ent_t;
struct state_stack_ent_t {
    pstate_t       state;      /** State of the parser */
    member_t*      member;     /** Current member being operated on */
    struct_def_t*  struct_def; /** */
    state_stack_ent_t* prev;   /** Previous entry on stack */
};



typedef struct {
    unsigned int      byte;   /** Current byte */
    unsigned int      line;   /** Current line */
    template_t*       templt; /** */
    wcapture_ctx_t    wcap;   /** Word capturer */
    state_stack_ent_t state;  /** Current state of parser */
} parse_ctx_t;

void parse_ctx_init(parse_ctx_t* ctx);
void parse_ctx_free(parse_ctx_t* ctx);
int parse_word_pre(parse_ctx_t* ctx, FILE* fp, int c);
int parse_word_post(parse_ctx_t* ctx, int c);

int parse_body(parse_ctx_t* ctx, int c);
int parse_comment_start(parse_ctx_t* ctx, int c);
int parse_comment_mline(parse_ctx_t* ctx, int c);
int parse_comment_mline_ast(parse_ctx_t* ctx, int c);
int parse_comment_sline(parse_ctx_t* ctx, int c);
int parse_word(parse_ctx_t* ctx, int c);
int parse_struct_name(parse_ctx_t* ctx, int c);
int parse_struct_body(parse_ctx_t* ctx, int c);
int parse_struct_field_type(parse_ctx_t* ctx, int c);
int parse_struct_field_name(parse_ctx_t* ctx, int c);
int parse_operator(parse_ctx_t* ctx, int c);
int parse_operator_assign(parse_ctx_t* ctx, int c);
int parse_var_decl(parse_ctx_t* ctx, int c);

typedef int (*parser_func_t)(parse_ctx_t* ctx, int c);
static parser_func_t parser_funcs[PSTATE_TOTAL] = {
    parse_body,
    parse_comment_start,
    parse_comment_mline,
    parse_comment_mline_ast,
    parse_comment_sline,
    parse_struct_name,
    parse_struct_body,
    parse_struct_field_type,
    parse_struct_field_name,
    parse_operator,
    parse_operator_assign,
    parse_var_decl,
};

int parse_template_file(template_t* t, const char* file);

#ifdef __cplusplus
}
#endif

#endif // TEMPLATE_PARSER_H
