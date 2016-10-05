#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mycroft/template.h>
#include "template_parser.h"

datatype_t* datatype_create(char* name) {

    datatype_t* datatype = NULL;

    /* Alloc datatype_t */
    datatype = (datatype_t*)malloc(sizeof(*datatype));
    if (datatype == NULL) {
        return NULL;
    }

    /* Copy name to datatype name */
    datatype->name = (char*)malloc(strlen(name)+1);
    if (datatype->name == NULL) {
        printf("Failed to allocate space for fs->name\n");
        return NULL;
    }
    strcpy(datatype->name, name);

    return datatype;
}

void datatype_free(datatype_t* type) {

    if (type->name != NULL) {
        free(type->name);
    }

    free(type);
}

ast_root_t* ast_root_create() {

    ast_root_t* ast_root = NULL;

    /* Alloc ast_root_t */
    ast_root = (ast_root_t*)malloc(sizeof(*ast_root));
    if (ast_root == NULL) {
        return NULL;
    }

    /* Init ast_root_t */
    ast_root->type = AST_TYPE_ROOT;
    ast_root->name = NULL;

    return ast_root;
}

void ast_root_free(ast_root_t* ast) {

    free(ast);
}

ast_var_t* ast_var_create() {

    ast_var_t* ast_var = NULL;

    /* Alloc ast_var_t */
    ast_var = (ast_var_t*)malloc(sizeof(*ast_var));
    if (ast_var == NULL) {
        return NULL;
    }

    /* Init ast_var_t */
    ast_var->type = AST_TYPE_VAR;
    ast_var->name = NULL;
    ast_var->datatype = NULL;

    return ast_var;
}

void ast_var_free(ast_var_t* ast) {

    if (ast->name != NULL) {
        free(ast->name);
    }

    if (ast->datatype != NULL) {
        datatype_free(ast->datatype);
    }

    free(ast);
}

ast_struct_t* ast_struct_create() {

    ast_struct_t* ast_struct = NULL;

    /* Alloc ast_struct_t */
    ast_struct = (ast_struct_t*)malloc(sizeof(*ast_struct));
    if (ast_struct == NULL) {
        return NULL;
    }

    /* Init ast_struct_t */
    ast_struct->type     = AST_TYPE_STRUCT;
    ast_struct->name     = NULL;
    ast_struct->nodes    = NULL;
    ast_struct->node_amt = 0;

    return ast_struct;
}

void ast_struct_free(ast_struct_t* ast) {

    if (ast->name != NULL) {
        free(ast->name);
    }

    if (ast->nodes != NULL) {
        int i = 0;
        for (; i < ast->node_amt; i++) {
            ast_node_t* node = ast->nodes[i];
            if (node->type == AST_TYPE_STRUCT) {
                ast_struct_free((ast_struct_t*)node);
            }
            else if (node->type == AST_TYPE_VAR) {
                ast_var_free((ast_var_t*)node);
            }
        }
    }

    free(ast);
}

int ast_struct_add_node(ast_struct_t* strct, ast_node_t* node) {

    /* Add space for new member */
    strct->nodes = (ast_node_t**)realloc(
        strct->nodes,
        (strct->node_amt+1)*sizeof(*strct->nodes));
    if (strct->nodes == NULL) {
        return -1;
    }

    /* Add member and increment total */
    strct->nodes[strct->node_amt] = node;
    strct->node_amt++;

    return 0;
}

member_t* member_create() {

    member_t* member = NULL;

    /* Alloc member_t */
    member = (member_t*)malloc(sizeof(*member));
    if (member == NULL) {
        return NULL;
    }

    /* Init member_t */
    member->name = NULL;
    member->type = NULL;

    return member;
}

void member_free(member_t* var) {

    if (var->name != NULL) {
        free(var->name);
    }

    if (var->type != NULL) {
        free(var->type);
    }

    free(var);
}

struct_def_t* struct_def_create() {

    struct_def_t* struct_def = NULL;

    /* Alloc struct_def_t */
    struct_def = (struct_def_t*)malloc(sizeof(*struct_def));
    if (struct_def == NULL) {
        return NULL;
    }

    /* Init struct_def_t */
    struct_def->name       = NULL;
    struct_def->members    = NULL;
    struct_def->member_amt = 0;

    return struct_def;
}

void struct_def_free(struct_def_t* sdef) {

    if (sdef->name != NULL) {
        free(sdef->name);
    }

    if (sdef->members != NULL) {
        int i = 0;
        for (; i < sdef->member_amt; i++) {
            member_free(sdef->members[i]);
        }
    }

    free(sdef);
}

int struct_def_add_member(struct_def_t* def, member_t* member) {

    /* Add space for new member */
    def->members = (member_t**)realloc(
        def->members,
        (def->member_amt+1)*sizeof(*def->members));
    if (def->members == NULL) {
        return -1;
    }

    /* Add member and increment total */
    def->members[def->member_amt] = member;
    def->member_amt++;

    return 0;
}

struct_def_t* template_find_struct_def(template_t* t, char* name) {

    int i = 0;
    for(; i < t->struct_def_amt; i++) {
        struct_def_t* sd = t->struct_defs[i];
        if (strcmp(sd->name, name) == 0) {
            return sd;
        }
    }

    return NULL;
}

ast_node_t* create_ast_node(template_t* t, ast_struct_t* parent, int index, member_t* member);

ast_struct_t* create_ast_struct(template_t* t, ast_struct_t* parent, int index, const char* name, struct_def_t* def) {

    int i = 0;
    ast_struct_t* ret = NULL;

    /* */
    ret = ast_struct_create();
    if (ret == NULL) {
        return NULL;
    }
    ret->parent = parent;
    ret->index = index++;

    /* */
    ret->name = (char*)malloc(strlen(name)+1);
    if (ret->name == NULL) {
        printf("Failed to allocate space for ret->name\n");
        return NULL;
    }
    strcpy(ret->name, name);

    /* */
    for (; i < def->member_amt; i++) {
        int rc = 0;
        ast_node_t* node = NULL;

        /* */
        node = create_ast_node(t, ret, 0, def->members[i]);
        if (node == NULL) {
            return NULL;
        }

        /* */
        rc = ast_struct_add_node(ret, node);
        if (rc < 0) {
            return NULL;
        }
    }

    return ret;
}

ast_var_t* create_ast_var(template_t* t, ast_struct_t* parent, int index, const char* name, const char* type) {

    ast_var_t* ret = NULL;

    /* */
    ret = ast_var_create();
    if (ret == NULL) {
        return NULL;
    }
    ret->parent = parent;
    ret->index = index++;

    /* */
    ret->name = (char*)malloc(strlen(name)+1);
    if (ret->name == NULL) {
        printf("Failed to allocate space for ret->name\n");
        return NULL;
    }
    strcpy(ret->name, name);

    /* */
    ret->datatype = datatype_create(type);
    if (ret->datatype == NULL) {
        printf("Failed to create datatype.\n");
        return NULL;
    }

    return ret;
}

ast_node_t* create_ast_node(template_t* t, ast_struct_t* parent, int index, member_t* member) {

    ast_node_t* ret = NULL;
    struct_def_t* def = NULL;

    def = template_find_struct_def(t, member->type);
    if (def != NULL) {
        ret = (ast_node_t*)create_ast_struct(t, parent, index, member->name, def);
    }
    else {
        ret = (ast_node_t*)create_ast_var(t, parent, index, member->name, member->type);
    }

    return ret;
}

int construct_ast(template_t* t) {

    struct_def_t* entry_def = NULL;
    ast_root_t* root = NULL;
    ast_struct_t* entry = NULL;

    /* Find entry */
    entry_def = template_find_struct_def(t, TPARSER_ENTRY);
    if (entry_def == NULL) {
        fprintf(stderr, "Failed to find entry struct in template.\n");
        return -1;
    }

    /* Create root */
    root = ast_root_create();
    if (root == NULL) {
        fprintf(stderr, "Failed to alloc ast_root_t.\n");
        return -1;
    }
    t->root = root;

    entry = create_ast_struct(t, NULL, 0, TPARSER_ENTRY, entry_def);
    if (entry == NULL) {
        fprintf(stderr, "Failed to create entry struct.");
        return -1;
    }
    entry->parent = root;
    entry->index = 0;
    root->entry = entry;

    return 0;
}

template_t* template_create() {

    template_t* template = NULL;

    /* Alloc template_t */
    template = (template_t*)malloc(sizeof(*template));
    if (template == NULL) {
        return NULL;
    }

    /* Init template_t */
    template->struct_defs    = NULL;
    template->struct_def_amt = 0;
    template->root           = NULL;

    return template;
}

template_t* template_create_from_file(const char* file) {

    int rc = 0;
    template_t* t = NULL;

    /* */
    t = template_create();
    if (t == NULL) {
        return NULL;
    }

    /* */
    rc = parse_template_file(t, file);
    if (rc < 0) {
        return NULL;
    }

    /* */
    rc = construct_ast(t);
    if (rc < 0) {
        return NULL;
    }

    return t;
}

void template_free(template_t* t) {

    if (t->struct_defs != NULL) {
        int i = 0;
        for (; i < t->struct_def_amt; i++) {
            struct_def_free(t->struct_defs[i]);
        }
    }

    if (t->root != NULL) {
        ast_root_free(t->root);
    }

    free(t);
}

int template_add_struct_def(template_t* t, struct_def_t* def) {

    /* Add space for new member */
    t->struct_defs = (struct_def_t**)realloc(
        t->struct_defs,
        (t->struct_def_amt+1)*sizeof(struct_def_t*));
    if (t->struct_defs == NULL) {
        return -1;
    }

    /* Add member and increment total */
    t->struct_defs[t->struct_def_amt] = def;
    t->struct_def_amt++;

    return 0;
}
