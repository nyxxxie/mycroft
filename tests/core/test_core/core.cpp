#include "gtest/gtest.h"
#include "mycroft/mycroft.h"


TEST(core_basic, mycroft_init_and_free) {
    mc_ctx_t* ctx = mycroft_init();
    ASSERT_TRUE(ctx != NULL);
    mycroft_free(ctx);
}

TEST(core_basic, mycroft_open_file_new) {
    mc_ctx_t* ctx = mycroft_init();
    ASSERT_TRUE(ctx != NULL);

    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin1"), 0);

    mycroft_free(ctx);
}

TEST(core_basic, mycroft_open_file_existing) {
    mc_ctx_t* ctx = mycroft_init();
    ASSERT_TRUE(ctx != NULL);

    ASSERT_EQ(mycroft_open_file(ctx, "res/testbin1"), 0);

    mycroft_free(ctx);
}
