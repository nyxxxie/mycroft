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

typedef struct mc_template_t mc_template_t;
mc_template_t* mc_template_create();
mc_template_t* mc_template_parse(const char* path);

/**
 * AST node types.
 */
typedef enum {
    AST_TYPE_ROOT = 0, /** Root node      */
    AST_TYPE_VAR,      /** Variable node. */
    AST_TYPE_ARRAY,    /** Variable node. */
    AST_TYPE_STRUCT    /** Struct node.   */
} ast_type_t;

typedef struct ast_node_t ast_node_t;
void mc_template_node_free(ast_node_t* node);
ast_type_t mc_template_node_get_type(ast_node_t* node);

typedef struct ast_struct_t ast_struct_t;
int mc_template_struct_get_field_amount(ast_struct_t* s);
ast_node_t* mc_template_struct_get_field(ast_struct_t* s, int index);

typedef struct ast_var_t ast_var_t;
const char* mc_template_var_get_type(ast_var_t* var);
const char* mc_template_var_get_name(ast_var_t* var);

typedef struct ast_array_t ast_array_t;
const char* mc_template_array_get_type(ast_var_t* var);
const char* mc_template_array_get_name(ast_var_t* var);
int mc_template_array_get_size(ast_var_t* var);


/**
 * This function gets a field from a struct using a syntax.
 *
 * EG:
 *   * "ENTRY.header.signature"
 *   * "ENTRY.body.somestructinbody."
 *
 * TODO: new name?
 */
ast_node_t* mc_template_get_field(const char* field);
ast_struct_t* mc_template_get_entry();

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_TEMPLATE_H
