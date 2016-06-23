#include "gtest/gtest.h"
#include "file.h"

TEST(test_file, file_exists_valid) {
    int res = file_exists("testfile1");
    ASSERT_EQ(res, 0);
}

TEST(test_file, file_exists_invalid) {
    int res = file_exists("thisfiledoesntexist");
    ASSERT_EQ(res, -1);
}
//TEST(test_file, file_exists_valid) {
//
//    mycroft_context_t* ctx = mycroft_init_cfg("adsfa");
//    if (ctx == NULL) {
//        fprintf(stderr, "Failed to create template struct!\n");
//        exit(1);
//    }
//    printf("Initialized core...\n");
//
//    ASSERT_EQ(ctx == NULL, 0);
//}
