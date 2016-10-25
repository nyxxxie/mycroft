#include <gtest/gtest.h>
#include <mycroft/mycroft.h>

/**
 * Ensure that we actually can allocate and free a ctx.  Hopefully this
 * test works!
 */
TEST(ctx_new, create_and_free_ctx) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    mc_ctx_free(ctx);
}

/**
 * Make sure that internal values are set properly.
 */
TEST(ctx_new, verify_default_values) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    ASSERT_TRUE(mc_ctx_get_focused_project(ctx) == NULL);
    ASSERT_TRUE(mc_ctx_get_project_amount(ctx) == 0);
    mc_ctx_free(ctx);
}

/**
 * Test to see if we can add ctxs to the context.  Also bonus test to make
 * sure we actually record the number of ctxs!
 */
TEST(ctx_new, add_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    ASSERT_TRUE(mc_ctx_add_project(ctx, projects[0]) == 0);
    ASSERT_TRUE(mc_ctx_add_project(ctx, projects[1]) == 1);
    ASSERT_TRUE(mc_ctx_add_project(ctx, projects[2]) == 2);

    ASSERT_TRUE(mc_ctx_get_project_amount(ctx) == 3);

    mc_ctx_free(ctx);
}

/**
 * Test to make adding a NULL project fails.
 */
TEST(ctx_new, add_null_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    ASSERT_TRUE(mc_ctx_add_project(ctx, NULL) == -1);

    mc_ctx_free(ctx);
}


/**
 * Check to make sure we can retrieve projects we add to the ctx.
 */
TEST(ctx_new, get_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    uint32_t projectind[3] = { 0 };
    projectind[0] = mc_ctx_add_project(ctx, projects[0]);
    projectind[1] = mc_ctx_add_project(ctx, projects[1]);
    projectind[2] = mc_ctx_add_project(ctx, projects[2]);

    ASSERT_TRUE(projectind[0] == 0);
    ASSERT_TRUE(projectind[1] == 1);
    ASSERT_TRUE(projectind[2] == 2);

    ASSERT_TRUE(projects[0] == mc_ctx_get_project(ctx, projectind[0]));
    ASSERT_TRUE(projects[1] == mc_ctx_get_project(ctx, projectind[1]));
    ASSERT_TRUE(projects[2] == mc_ctx_get_project(ctx, projectind[2]));

    mc_ctx_free(ctx);
}

/**
 * Check to make sure we can't retrieve projects we didn't add to the project.
 */
TEST(project_new, get_null_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    ASSERT_TRUE(mc_ctx_get_project(ctx, 0) == NULL);
    ASSERT_TRUE(mc_ctx_get_project(ctx, 1) == NULL);
    ASSERT_TRUE(mc_ctx_get_project(ctx, 123) == NULL);
    mc_ctx_free(ctx);
}

/**
 * Check to make sure we can remove projects we add to the ctx.
 */
TEST(ctx_new, remove_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    mc_project_t* projects[3] = {NULL};
    ASSERT_TRUE((projects[0] = mc_project_create("project1")) != NULL);
    ASSERT_TRUE((projects[1] = mc_project_create("project2")) != NULL);
    ASSERT_TRUE((projects[2] = mc_project_create("project3")) != NULL);

    uint32_t projectind[3] = { 0 };
    projectind[0] = mc_ctx_add_project(ctx, projects[0]);
    projectind[1] = mc_ctx_add_project(ctx, projects[1]);
    projectind[2] = mc_ctx_add_project(ctx, projects[2]);

    ASSERT_TRUE(projectind[0] == 0);
    ASSERT_TRUE(projectind[1] == 1);
    ASSERT_TRUE(projectind[2] == 2);

    ASSERT_TRUE(mc_ctx_remove_project(ctx, projectind[0]) == 0);
    ASSERT_TRUE(mc_ctx_remove_project(ctx, projectind[1]) == 0);
    ASSERT_TRUE(mc_ctx_remove_project(ctx, projectind[2]) == 0);

    ASSERT_TRUE(mc_ctx_get_project_amount(ctx) == 0);

    mc_ctx_free(ctx);
}

/**
 * Test focused project functionality.
 */
TEST(ctx_new, focused_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    mc_project_t* project = NULL;
    ASSERT_TRUE((project = mc_project_create("project1")) != NULL);

    uint32_t i = mc_ctx_add_project(ctx, project);
    ASSERT_TRUE(i == 0);

    mc_ctx_set_focused_project(ctx, project);
    ASSERT_TRUE(project == mc_ctx_get_focused_project(ctx));

    ASSERT_TRUE(mc_ctx_remove_project(ctx, i) == 0);

    mc_ctx_free(ctx);
}

// TODO: add project load tests
