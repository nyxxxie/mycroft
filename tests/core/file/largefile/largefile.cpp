#include "gtest/gtest.h"
#include "file.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* NOTE: please fix the tests! */

/**
 * Tests large file seeking
 */
TEST(file_largefile, file_seeking) {
    mc_file_t* f = mc_file_open("largesparsefile");
    ASSERT_TRUE(f != NULL);
    ASSERT_EQ(mc_file_set_cursor(f, (fsize_t) 0x100000000), 0);
    ASSERT_EQ(mc_file_close(f), 0);
}


/**
 * Tests large file reading
 */
TEST(file_largefile, file_seeking) {
    mc_file_t* f = mc_file_open("largesparsefile");
    uint8_t outbuf[6];
    ASSERT_TRUE(f != NULL);
    ASSERT_EQ(mc_file_set_cursor(f, (fsize_t) 0x100000000), 0);
    ASSERT_EQ(mc_file_read(f, sizeof(outbuf), outbuf), 0);
    ASSERT_EQ(memcmp("hello!", outbuf, sizeof(outbuf)), 0);
    ASSERT_EQ(mc_file_close(f), 0);
}
