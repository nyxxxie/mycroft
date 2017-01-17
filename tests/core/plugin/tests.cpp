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
