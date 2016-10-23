#include <gtest/gtest.h>
#include <mycroft/mycroft.h>

TEST(context, create_and_free_ctx) {
    mc_ctx_t* ctx = mc_ctx_create();
    ASSERT_TRUE(ctx != NULL);
    mc_ctx_free(ctx);
}
