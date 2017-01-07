#include <gtest/gtest.h>
#include <mycroft/mycroft.h>

/**
 * Ensure that we actually can allocate and free a ctx.  Hopefully this
 * test works!
 */
TEST(ctx_new, create_and_free_ctx) {
    ASSERT_TRUE(mc_init() == MC_OK);
    mc_destroy();
}

/**
 * Make sure that internal values are set properly.
 */
TEST(ctx_new, verify_default_values) {
    ASSERT_TRUE(mc_init() == MC_OK);
    ASSERT_TRUE(mc_get_focused_project() == NULL);
    ASSERT_TRUE(mc_get_project_amount() == 0);
    mc_destroy();
}

/**
 * Test to see if we can add ctxs to the context.  Also bonus test to make
 * sure we actually record the number of ctxs!
 */
TEST(ctx_new, add_project) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_add_project(projects[0]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[1]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[2]) == MC_OK);

    ASSERT_TRUE(mc_get_project_amount() == 3);

    mc_destroy();
}

/**
 * Test to make adding a NULL project fails.
 */
TEST(ctx_new, add_null_project) {
    ASSERT_TRUE(mc_init() == MC_OK);
    ASSERT_TRUE(mc_add_project(NULL) != MC_OK);
    mc_destroy();
}


/**
 * Check to make sure we can retrieve projects we add to the ctx.
 */
TEST(ctx_new, get_project) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_add_project(projects[0]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[1]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[2]) == MC_OK);

    ASSERT_TRUE(projects[0] == mc_get_project(0));
    ASSERT_TRUE(projects[1] == mc_get_project(1));
    ASSERT_TRUE(projects[2] == mc_get_project(2));

    mc_destroy();
}

/**
 * Check to make sure we can't retrieve projects we didn't add to the project.
 */
TEST(project_new, get_null_project) {
    ASSERT_TRUE(mc_init() == MC_OK);
    ASSERT_TRUE(mc_get_project(0) == NULL);
    ASSERT_TRUE(mc_get_project(1) == NULL);
    ASSERT_TRUE(mc_get_project(123) == NULL);
    mc_destroy();
}

/**
 * Check to make sure we can remove files we add to the project.
 */
TEST(project_new, remove_project) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_add_project(projects[0]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[1]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[2]) == MC_OK);

    ASSERT_TRUE(mc_remove_project(2) == MC_OK);
    ASSERT_TRUE(mc_remove_project(1) == MC_OK);
    ASSERT_TRUE(mc_remove_project(0) == MC_OK);

    ASSERT_TRUE(mc_get_project_amount() == 0);

    mc_destroy();
}

/**
 * Check in a different way if we can remove files we add to the project.
 */
TEST(project_new, remove_file_2) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_add_project(projects[0]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[1]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[2]) == MC_OK);

    ASSERT_TRUE(mc_remove_project(0) == MC_OK);
    ASSERT_TRUE(mc_remove_project(0) == MC_OK);
    ASSERT_TRUE(mc_remove_project(0) == MC_OK);

    ASSERT_TRUE(mc_get_project_amount() == 0);

    mc_destroy();
}

/**
 * Check to make sure that the remove function shifts over elements
 */
TEST(project_new, remove_shiftover) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_add_project(projects[0]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[1]) == MC_OK);
    ASSERT_TRUE(mc_add_project(projects[2]) == MC_OK);

    ASSERT_TRUE(projects[1] == mc_get_project(1));
    ASSERT_TRUE(mc_remove_project(1) == MC_OK);
    ASSERT_TRUE(projects[2] == mc_get_project(1));

    mc_destroy();
}

/**
 * Test focused project functionality.
 */
TEST(ctx_new, focused_project) {
    ASSERT_TRUE(mc_init() == MC_OK);

    mc_project_t* project = NULL;
    ASSERT_TRUE((project = mc_project_create("project1")) != NULL);

    ASSERT_TRUE(mc_add_project(project) == MC_OK);

    mc_set_focused_project(project);
    ASSERT_TRUE(project == mc_get_focused_project());

    ASSERT_TRUE(mc_remove_project(0) == MC_OK);

    mc_destroy();
}

// TODO: add project load tests
