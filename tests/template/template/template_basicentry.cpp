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

#include <mycroft/template.h>
#include "gtest/gtest.h"
#include <stdio.h>
#include <string.h>

/**
 * Try opening and parsing the template, then closing it.
 */
TEST(template_basicentry, open_template) {

    template_t* t = NULL;

    /* Open template */
    t = template_create_from_file("res/basicentry.mtf");
    ASSERT_TRUE(t != NULL);

    /* Cleanup */
    template_free(t);
}
