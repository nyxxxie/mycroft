#include "gtest/gtest.h"
#include "config.h"
#include <stdio.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("%s\n", TEST_FILE_RES);
    return RUN_ALL_TESTS();
}
