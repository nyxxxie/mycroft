/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

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
    // copyfile(
    //     std::string(TEST_FILE_SRC_RES) + "/" + file,
    //     std::string(TEST_FILE_BIN_RES) + "/" + file);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    refreshfile("longboringdocument.txt");
    refreshfile("testfile1");
    return RUN_ALL_TESTS();
}
