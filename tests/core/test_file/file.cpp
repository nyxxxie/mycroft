#include "gtest/gtest.h"
#include "file.h"

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

//TODO: refactor file with no res variable and fix tests.

TEST(file_basic, file_exists_valid) {
    ASSERT_EQ(file_exists("res/testfile1"), 0);
}

TEST(file_basic, file_exists_invalid) {
    ASSERT_EQ(file_exists("thisfiledoesntexist"), -1);
}

TEST(file_basic, file_init) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
}

TEST(file_basic, file_open_valid) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);
    ASSERT_EQ(file_close(&m), 0);
}

TEST(file_basic, file_open_invalid) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, "thisfiledoesntexist"), 0);
}

TEST(file_basic, file_open_emptystr) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, ""), 0);
}

TEST(file_basic, file_open_nullstr) {
    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_LT(file_open(&m, NULL), 0);
}

TEST(file_mgmt, file_get_cursor) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_EQ(file_get_cursor(&m), 0);

    ASSERT_EQ(file_close(&m), 0);
}

TEST(file_mgmt, file_set_cursor) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_GE(file_set_cursor(&m, 5), 0);

    ASSERT_EQ(file_get_cursor(&m), 5);

    ASSERT_EQ(file_close(&m), 0);
}

TEST(file_mgmt, file_set_cursor_invalid_value) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_LT(file_set_cursor(&m, 9999), 0);

    ASSERT_EQ(file_close(&m), 0);
}

TEST(file_mgmt, file_cursor_manip) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    ASSERT_EQ(file_set_cursor(&m, 0), 0);

    ASSERT_EQ(file_close(&m), 0);
}

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


TEST(file_read, file_read_value_bad_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[4] = {0};
    ASSERT_LT(file_read_value(&m, 999, 4, recieved), 0);

    ASSERT_EQ(file_close(&m), 0);
}

TEST(file_read, file_read_value_off_end) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t recieved[4] = {0};
    ASSERT_LT(file_read_value(&m, 30, 4, recieved), 0);

    ASSERT_EQ(file_close(&m), 0);
}

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

TEST(file_read, file_write_value_bad_offset) {

    mc_file_t m;
    ASSERT_EQ(file_init(&m), 0);
    ASSERT_EQ(file_open(&m, "res/testfile1"), 0);

    uint8_t bytes[] = {0xaa, 0xaa, 0xaa};
    ASSERT_LT(file_write_value(&m, 9999, sizeof(bytes), bytes), 0);

    ASSERT_EQ(file_close(&m), 0);
}
