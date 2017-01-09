#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "config.h"

void copyfile(std::string src, std::string dest) {
    std::ifstream srcs(src.c_str(), std::ios::binary);
    std::ofstream dests(dest.c_str(), std::ios::binary);
    dests << srcs.rdbuf();
}

void refreshfile(std::string file) {
    copyfile(
        std::string(TEST_FILE_SRC_RES) + "/" + file,
        std::string(TEST_FILE_BIN_RES) + "/" + file);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    refreshfile("longboringdocument.txt");
    refreshfile("testfile1");
    return RUN_ALL_TESTS();
}
