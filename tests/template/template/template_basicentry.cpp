#include <mycroft/template.h>
#include "gtest/gtest.h"
#include <stdio.h>
#include <string.h>

/**
 * Try opening and parsing the template, then closing it.
 */
TEST(template_basicentry, open_template) {

    template_t* t = NULL;

    /* Open template */
    t = template_create_from_file("res/basicentry.mtf");
    ASSERT_TRUE(t != NULL);

    /* Cleanup */
    template_free(t);
}
