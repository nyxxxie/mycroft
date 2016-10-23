#include <stdio.h>
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    remove("res/testbin1.mdb");
    remove("res/testbin2.mdb");
    return RUN_ALL_TESTS();
}
