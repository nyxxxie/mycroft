#include "gtest/gtest.h"
#include "template.h"
#include "template_parser.h"

#include <stdio.h>
#include <string.h>

/**
 * Try opening and parsing the template, then closing it.
 */
TEST(template_somestructs, open_template) {

    template_t* t = NULL;

    /* Open template */
    t = template_create_from_file("res/somestructs.mtf");
    ASSERT_TRUE(t != NULL);

    /* Cleanup */
    template_free(t);
}

/**
 * Check the validity of the entry struct.
 */
TEST(template_somestructs, check_entry) {

    template_t* t = NULL;
    ast_struct_t* entry = NULL;

    /* Open template */
    t = template_create_from_file("res/somestructs.mtf");
    ASSERT_TRUE(t != NULL);

    /* Get entry struct */
    entry = t->entry;
    ASSERT_TRUE(entry != NULL);

    /* Since this is the entry node, there should be no parent */
    ASSERT_TRUE(entry->parent == NULL);

    /* Check to make sure we have exactly three nodes */
    ASSERT_EQ(entry->node_amt, 3);

    /* Check entry 1 */
    ASSERT_EQ(entry->nodes[0]->type, AST_TYPE_VAR);
    ASSERT_EQ(strcmp(entry->nodes[0]->name, "blah"), 0);

    /* Check entry 1 */
    ASSERT_EQ(entry->nodes[1]->type, AST_TYPE_STRUCT);
    ASSERT_EQ(strcmp(entry->nodes[1]->name, "s1"), 0);

    /* Check entry 1 */
    ASSERT_EQ(entry->nodes[2]->type, AST_TYPE_STRUCT);
    ASSERT_EQ(strcmp(entry->nodes[2]->name, "s2"), 0);

    /* Cleanup */
    template_free(t);
}

/**
 * Check the validity of the entry struct's first struct.
 */
TEST(template_somestructs, check_struct1) {

    template_t* t = NULL;
    ast_struct_t* strct = NULL;

    /* Open template */
    t = template_create_from_file("res/somestructs.mtf");
    ASSERT_TRUE(t != NULL);

    /* Get entry struct */
    strct = (ast_struct_t*)t->entry->nodes[1];
    ASSERT_TRUE(strct != NULL);

    /* Check to make sure that the entry node is this node's parent */
    ASSERT_TRUE(strct->parent == (ast_node_t*)t->entry);

    /* Check to make sure we have exactly two nodes */
    ASSERT_EQ(strct->node_amt, 2);

    /* Check entry 1 */
    ASSERT_EQ(strct->nodes[0]->type, AST_TYPE_VAR);
    ASSERT_EQ(strcmp(strct->nodes[0]->name, "attribute1"), 0);

    /* Check entry 2 */
    ASSERT_EQ(strct->nodes[1]->type, AST_TYPE_VAR);
    ASSERT_EQ(strcmp(strct->nodes[1]->name, "attribute2"), 0);

    /* Cleanup */
    template_free(t);
}

/**
 * Check the validity of the entry struct's second struct.
 */
TEST(template_somestructs, check_struct2) {

    template_t* t = NULL;
    ast_struct_t* strct = NULL;

    /* Open template */
    t = template_create_from_file("res/somestructs.mtf");
    ASSERT_TRUE(t != NULL);

    /* Get entry struct */
    strct = (ast_struct_t*)t->entry->nodes[2];
    ASSERT_TRUE(strct != NULL);

    /* Check to make sure that the entry node is this node's parent */
    ASSERT_TRUE(strct->parent == (ast_node_t*)t->entry);

    /* Check to make sure we have exactly two nodes */
    ASSERT_EQ(strct->node_amt, 2);

    /* Check entry 1 */
    ASSERT_EQ(strct->nodes[0]->type, AST_TYPE_VAR);
    ASSERT_EQ(strcmp(strct->nodes[0]->name, "attribute1"), 0);

    /* Check entry 2 */
    ASSERT_EQ(strct->nodes[1]->type, AST_TYPE_VAR);
    ASSERT_EQ(strcmp(strct->nodes[1]->name, "attribute2"), 0);

    /* Cleanup */
    template_free(t);
}
