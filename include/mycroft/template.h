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

#ifndef MYCROFT_TEMPLATE_H
#define MYCROFT_TEMPLATE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Placeholder until type system gets completed.
 */
typedef struct {
    char* name;
} datatype_t;

datatype_t* datatype_create(const char* name);
void datatype_free(datatype_t* type);

///////////////////////////////////////////////////////////////////////////////

/**
 * AST node types.
 */
typedef enum {
    AST_TYPE_ROOT = 0, /** Root node      */
    AST_TYPE_VAR,      /** Variable node. */
    AST_TYPE_STRUCT    /** Struct node.   */
} ast_type_t;

/**
 * Generic node in the abstract syntax tree.  The ast is meant to be used
 * directly to parse a file
 */
typedef struct {
    int        index;
    ast_type_t type;
    char*      name;
} ast_node_t;

typedef struct ast_struct_t ast_struct_t; // forward decl

/**
 * Var in the ast.
 */
typedef struct {
    int           index;
    ast_type_t    type;
    char*         name;
    ast_struct_t* parent;
    datatype_t*   datatype;
} ast_var_t;

ast_var_t* ast_var_create();
void ast_var_free(ast_var_t* ast);

/**
 * Struct in the ast.
 */
struct ast_struct_t {
    int           index;
    ast_type_t    type;
    char*         name;
    ast_struct_t* parent;
    ast_node_t**  nodes;
    unsigned int  node_amt;
};

/**
 * Root of the ast.
 */
typedef struct {
    int           index;
    ast_type_t    type;
    char*         name;
    ast_struct_t* entry;
} ast_root_t;

ast_root_t* ast_root_create();
void ast_root_free(ast_root_t* ast);

ast_struct_t* ast_struct_create();
void ast_struct_free(ast_struct_t* ast);
int ast_struct_add_node(ast_struct_t* strct, ast_node_t* node);

///////////////////////////////////////////////////////////////////////////////

typedef struct {
    char* name;
    char* type;
} member_t;

member_t* member_create();
void member_free(member_t* var);

/**
 * Represents a structure defined by the template file.  Used to create ast
 * struct nodes.
 */
typedef struct {
    char*         name;
    member_t**    members;
    unsigned int  member_amt;
} struct_def_t;

struct_def_t* struct_def_create();
void struct_def_free(struct_def_t* sdef);
int struct_def_add_member(struct_def_t* def, member_t* member);

///////////////////////////////////////////////////////////////////////////////

/**
 * Contains information on a template.
 */
typedef struct {
    struct_def_t** struct_defs;
    unsigned int   struct_def_amt;
    ast_root_t*    root;
} template_t;

template_t* template_create();
template_t* template_create_from_file(const char* file);
void template_free(template_t* t);
int template_add_struct_def(template_t* t, struct_def_t* def);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_TEMPLATE_H
