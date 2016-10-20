#include <gtest/gtest.h>
#include <mycroft/mycroft.h>

TEST(context, create_ctx) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
}

TEST(context, create_and_free_ctx) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    mc_ctx_free(ctx);
}

TEST(project, create_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    mc_ctx_free(ctx);
}

TEST(project, create_and_free_project) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    mc_ctx_free(ctx);
}

TEST(project_new, add_project_to_ctx) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);

    mc_project_t* project = mc_project_create("name");
    mc_ctx_add_project(ctx, project);


    mc_ctx_free(ctx);
}

TEST(project_new, add_many_projects_to_ctx) {
}

TEST(project_new, verify_project_is_stored) {
}

TEST(project_new, verify_projects_are_stored) {
}

//TEST(core_basic, mycroft_init_and_free) {
//    mc_ctx_t* ctx = mycroft_init();
//    ASSERT_TRUE(ctx != NULL);
//    mycroft_free(ctx);
//}
//
//TEST(core_basic, mycroft_open_file_new) {
//    mc_ctx_t* ctx = mycroft_init();
//    ASSERT_TRUE(ctx != NULL);
//
//    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin1"), 0);
//
//    mycroft_free(ctx);
//}
//
//TEST(core_basic, mycroft_open_file_existing) {
//    mc_ctx_t* ctx = mycroft_init();
//    ASSERT_TRUE(ctx != NULL);
//
//    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin1"), 0);
//
//    mycroft_free(ctx);
//}
//
//TEST(core_basic, mycroft_open_file_new_verify) {
//    mc_ctx_t* ctx = mycroft_init();
//    ASSERT_TRUE(ctx != NULL);
//
//    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin2"), 0);
//    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin2"), 0);
//
//    mycroft_free(ctx);
//}
//
//TEST(core_basic, mycroft_open_file_existing_verify) {
//    mc_ctx_t* ctx = mycroft_init();
//    ASSERT_TRUE(ctx != NULL);
//
//    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin2"), 0);
//
//    mycroft_free(ctx);
//}
