#include "gtest/gtest.h"
#include "file.h"

#include <stdio.h>
#include <string.h>

TEST(file_basic, file_exists_valid) {
    int res = file_exists("res/testfile1");
    ASSERT_EQ(res, 0);
}

TEST(file_basic, file_exists_invalid) {
    int res = file_exists("thisfiledoesntexist");
    ASSERT_EQ(res, -1);
}

TEST(file_basic, file_open_valid) {
    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);
}

TEST(file_basic, file_open_invalid) {
    mc_file_t m;
    int res = file_open(&m, "thisfiledoesntexist");
    ASSERT_LT(res, 0);
}

TEST(file_basic, file_open_emptystr) {
    mc_file_t m;
    int res = file_open(&m, "");
    ASSERT_LT(res, 0);
}

TEST(file_basic, file_open_nullstr) {
    mc_file_t m;
    int res = file_open(&m, NULL);
    ASSERT_LT(res, 0);
}

TEST(file_mgmt, file_get_cursor) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    int c = file_get_cursor(&m);
    ASSERT_EQ(c, 0);
}

TEST(file_mgmt, file_set_cursor) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    res = file_set_cursor(&m, 5);
    ASSERT_GE(res, 0);

    ASSERT_EQ(file_get_cursor(&m), 5);
}

TEST(file_mgmt, file_set_cursor_invalid_pos) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    res = file_set_cursor(&m, 9999);
    ASSERT_LT(res, 0);
}

TEST(file_mgmt, file_cursor_manip) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    res = file_set_cursor(&m, 0);
    ASSERT_EQ(res, 0);
}

TEST(file_read, file_read_pos) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t recieved[4];
    res = file_read_pos(&m, 0, 4, recieved);
    ASSERT_EQ(res, 0);

    uint8_t expected[] = {0x61, 0x62, 0x63, 0x64};
    ASSERT_EQ(memcmp(expected, recieved, 4), 0);
}

TEST(file_read, file_read_pos_offset) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t recieved[3] = {0};
    res = file_read_pos(&m, 3, 3, recieved);
    ASSERT_EQ(res, 0);

    uint8_t expected[] = {0x64, 0x65, 0x66};
    ASSERT_EQ(memcmp(expected, recieved, 3), 0);
}

TEST(file_read, file_read_pos_multi) {

    uint8_t expected[] = {0x61, 0x62, 0x63, 0x64};

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    /* Perform first read */
    uint8_t recieved[4];
    res = file_read_pos(&m, 0, 4, recieved);
    ASSERT_EQ(res, 0);

    ASSERT_EQ(memcmp(expected, recieved, 4), 0);

    /* Perform second read */
    res = file_read_pos(&m, 0, 4, recieved);
    ASSERT_EQ(res, 0);

    ASSERT_EQ(memcmp(expected, recieved, 4), 0);
}


TEST(file_read, file_read_pos_bad_offset) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t recieved[4] = {0};
    res = file_read_pos(&m, 999, 4, recieved);
    ASSERT_LT(res, 0);
}

TEST(file_read, file_read_pos_off_end) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t recieved[4] = {0};
    res = file_read_pos(&m, 30, 4, recieved);
    ASSERT_LT(res, 0);
}

TEST(file_read, file_write_pos) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t bytes[] = {0xff, 0xff, 0xff};
    res = file_write_pos(&m, 0, sizeof(bytes), bytes);
    ASSERT_EQ(res, 0);

    uint8_t recieved[3] = {0};
    res = file_read_pos(&m, 0, sizeof(bytes), recieved);
    ASSERT_EQ(res, 0);

    ASSERT_EQ(memcmp(bytes, recieved, sizeof(bytes)), 0);
}

TEST(file_read, file_write_pos_offset) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t bytes[] = {0xbb, 0xbb, 0xbb};
    res = file_write_pos(&m, 5, sizeof(bytes), bytes);
    ASSERT_EQ(res, 0);

    uint8_t recieved[3] = {0};
    res = file_read_pos(&m, 5, sizeof(bytes), recieved);
    ASSERT_EQ(res, 0);

    ASSERT_EQ(memcmp(bytes, recieved, sizeof(bytes)), 0);
}

TEST(file_read, file_write_pos_bad_offset) {

    mc_file_t m;
    int res = file_open(&m, "res/testfile1");
    ASSERT_EQ(res, 0);

    uint8_t bytes[] = {0xaa, 0xaa, 0xaa};
    res = file_write_pos(&m, 9999, sizeof(bytes), bytes);
    ASSERT_LT(res, 0);
}
