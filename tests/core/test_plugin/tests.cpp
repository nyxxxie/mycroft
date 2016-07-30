#include "gtest/gtest.h"

//TODO: Don't test the init/free functions, just test the functions they call.
//      Reason for this is that we don't want to load plugins from the actual
//      plugin path.

/**
 *
 */
TEST(plugin, aaa) {
    //ASSERT_EQ(file_exists("res/testfile1"), 0);
    ASSERT_TRUE(true);
}
