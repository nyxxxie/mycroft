#include "gtest/gtest.h"
#include "file.h"

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

/**
 * Tests file_exist function on a existing file.
 */
TEST(file_basic, file_exists_valid) {
    ASSERT_EQ(file_exists("res/testfile1"), 0);
}

/**
 * Tests file_exist function on a nonexistant file.
 */
TEST(file_basic, file_exists_invalid) {
    ASSERT_EQ(file_exists("thisfiledoesntexist"), -1);
}

/**
 * Tests to see if the file_init function works
 */
TEST(file_basic, file_init) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
}

/**
 * Open an existing file
 */
TEST(file_basic, file_open_valid) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Open a nonexistant file
 */
TEST(file_basic, file_open_invalid) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, "thisfiledoesntexist"), 0);
}

/**
 * See how file_init handles an empty string as a file path
 */
TEST(file_basic, file_open_emptystr) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, ""), 0);
}

/**
 * See how file_init handles NULL as a file path
 */
TEST(file_basic, file_open_nullstr) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, NULL), 0);
}

/**
 * See if we can get the cursor position
 */
TEST(file_mgmt, file_get_cursor) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_EQ(file_get_cursor(&m), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if we can set the cursor position
 */
TEST(file_mgmt, file_set_cursor) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_GE(file_set_cursor(&m, 5), 0);

    ASSERT_EQ(file_get_cursor(&m), 5);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See what happens if we set the cursor to a position waaaay off the end of
 * the file.
 */
TEST(file_mgmt, file_set_cursor_invalid_value) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_LT(file_set_cursor(&m, 9999), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Some more testing on file_set_cursor.
 */
TEST(file_mgmt, file_cursor_manip) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_EQ(file_set_cursor(&m, 0), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if the file_read_value reads properly.
 */
TEST(file_read, file_read_value) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[4];
    ASSERT_EQ(file_read_value(&m, 0, 4, recieved), 0);

    uint8_t expected[] = {0x61, 0x62, 0x63, 0x64};
    ASSERT_EQ(memcmp(expected, recieved, 4), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if the file_read reads properly.
 */
TEST(file_read, file_read) {

    const unsigned char expected_hash[] = {
        0x5e, 0x2e, 0x5f, 0x96, 0xb9, 0xfc, 0x4d, 0xe7, 0x31, 0xb3,
        0x7e, 0xcd, 0xa6, 0xd4, 0x3f, 0x6b, 0x4f, 0x97, 0xf0, 0x42
    };

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/longboringdocument.txt"), 0);

    SHA_CTX ctx;
    SHA1_Init(&ctx);

    uint8_t readbuf[32];
    while (1) {
        int amnt = file_read(&m, sizeof(readbuf), readbuf);
        ASSERT_GE(amnt, 0);
        if (amnt == 0) {
            break;
        }
        SHA1_Update(&ctx, readbuf, amnt);
    }

    unsigned char hash[20];
    SHA1_Final(hash, &ctx);

    ASSERT_EQ(memcmp(hash, expected_hash, 4), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if the file_read reads properly when we provide an offset.
 */
TEST(file_read, file_read_value_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[3] = {0};
    ASSERT_EQ(file_read_value(&m, 3, 3, recieved), 0);

    uint8_t expected[] = {0x64, 0x65, 0x66};
    ASSERT_EQ(memcmp(expected, recieved, 3), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if file_read_value withstands the punishment of multiple invocations
 * on the same file.
 */
TEST(file_read, file_read_value_multi) {

    uint8_t expected[] = {0x61, 0x62, 0x63, 0x64};

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    /* Perform first read */
    uint8_t recieved[4];
    ASSERT_EQ(file_read_value(&m, 0, 4, recieved), 0);
    ASSERT_EQ(memcmp(expected, recieved, 4), 0);

    /* Perform second read */
    ASSERT_EQ(file_read_value(&m, 0, 4, recieved), 0);
    ASSERT_EQ(memcmp(expected, recieved, 4), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * See if file_read_value bursts into flames gracefully if we feed it a
 * rediculous offset.
 */
TEST(file_read, file_read_value_bad_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[4] = {0};
    ASSERT_LT(file_read_value(&m, 999, 4, recieved), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Make sure that file_read_value won't attempt to read values past the file's
 * end.
 */
TEST(file_read, file_read_value_off_end) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[4] = {0};
    ASSERT_LT(file_read_value(&m, 30, 4, recieved), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try and write a value
 */
TEST(file_read, file_write_value) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t bytes[] = {0xff, 0xff, 0xff};
    ASSERT_EQ(file_write_value(&m, 0, sizeof(bytes), bytes), 0);

    uint8_t recieved[3] = {0};
    ASSERT_EQ(file_read_value(&m, 0, sizeof(bytes), recieved), 0);

    ASSERT_EQ(memcmp(bytes, recieved, sizeof(bytes)), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try and write a value at an offset
 */
TEST(file_read, file_write_value_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t bytes[] = {0xbb, 0xbb, 0xbb};
    ASSERT_EQ(file_write_value(&m, 5, sizeof(bytes), bytes), 0);

    uint8_t recieved[3] = {0};
    ASSERT_EQ(file_read_value(&m, 5, sizeof(bytes), recieved), 0);

    ASSERT_EQ(memcmp(bytes, recieved, sizeof(bytes)), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try and write a value at an offset that doesn't exist (spoiler alert: it
 * should fail)
 */
TEST(file_read, file_write_value_bad_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t bytes[] = {0xaa, 0xaa, 0xaa};
    ASSERT_LT(file_write_value(&m, 9999, sizeof(bytes), bytes), 0);

    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try to create the cache
 */
TEST(file_cache, file_cache_init) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
    ASSERT_EQ(file_cache_init(&m, 32), 0);
    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try to create the cache (with a really big size)
 */
TEST(file_cache, file_cache_init_big) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
    ASSERT_EQ(file_cache_init(&m, 1024), 0);
    ASSERT_EQ(file_close(&m), 0);
}

/**
 * Try to create the cache (with a really small size)
 */
TEST(file_cache, file_cache_init_tiny) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
    ASSERT_EQ(file_cache_init(&m, 8), 0);
    ASSERT_EQ(file_close(&m), 0);
}

///**
// * Try loading bytes into the cache
// */
//TEST(file_cache, file_cache_loadzone) {
//    mc_file_t m;
//    ASSERT_EQ(file_init(&m), 0);
//    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
//    ASSERT_EQ(file_cache_init(&m, 8));
//    ASSERT_EQ(file_free(&m), 0);
//}
//
///**
// * Try loading bytes into a really big cache
// */
//TEST(file_cache, file_cache_loadzone_big) {
//
//}
//
///**
// * Try loading bytes such that we'll end up reading past the file
// */
//TEST(file_cache, file_cache_loadzone_outofbounds) {
//
//}
//
///**
// * Try reloading bytes into cache
// */
//TEST(file_cache, file_cache_reload) {
//
//}
//
///**
// * Read bytes from cache.
// */
//TEST(file_cache, file_cache_read_full) {
//
//}
//
///**
// * Read bytes such that we read from both the cache and the file itself.
// */
//TEST(file_cache, file_cache_read_partial) {
//
//}
//
///**
// * Write bytes into cache.
// */
//TEST(file_cache, file_cache_write_full) {
//
//}
//
///**
// * Write bytes into cache such that they end up in both cached and uncached
// * areas.
// */
//TEST(file_cache, file_cache_write_partial) {
//
//}
