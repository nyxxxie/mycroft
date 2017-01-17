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
    ASSERT_EQ(mc_file_set_cursor(f, (fsize_t) 0x100000000), MC_OK);
    ASSERT_EQ(mc_file_close(f), MC_OK);
}


/**
 * Tests large file reading
 */
TEST(file_largefile, file_seeking) {
    mc_file_t* f = mc_file_open("largesparsefile");
    uint8_t outbuf[6];
    ASSERT_TRUE(f != NULL);
    ASSERT_EQ(mc_file_set_cursor(f, (fsize_t) 0x100000000), MC_OK);
    ASSERT_EQ(mc_file_read(f, sizeof(outbuf), outbuf, NULL), MC_OK);
    ASSERT_EQ(memcmp("hello!", outbuf, sizeof(outbuf)), 0);
    ASSERT_EQ(mc_file_close(f), MC_OK);
}
