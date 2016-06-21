#include "gtest/gtest.h"

TEST(test, add) {
    int res = 2 + 2;
    ASSERT_EQ(res, 4);
}

TEST(test, sub) {
    int res = 2 - 2;
    ASSERT_EQ(res, 0);
}

TEST(test, thiswillfail) {
    int res = 2 - 2;
    ASSERT_EQ(res, 5);
}
