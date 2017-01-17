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

#include <stdio.h>
#include "config.h"
#include <mycroft/mycroft.h>
#include <mycroft/plugin.h>
#include <mycroft/script.h>

int main(int argc, char* argv[])
{
    mc_project_t* project = NULL;

    /* Init mycroft */
    if (mc_init() == MC_ERR) {
        printf("Failed to initialize mycroft.");
        return 1;
    }

    /* Checked to see if a file was specified */
    if (argc < 1) {
        printf("Script file not specified.\n");
        return 1;
    }

    /* Create project */
    project = mc_project_create("test project");
    if (project == NULL) {
        fprintf(stderr, "Failed to create project, exiting...\n");
        mc_destroy();
        return 1;
    }

    printf("+-------------: Ran script \"%s\"\n", argv[1]);
    if (mc_script_runfile(argv[1]) != MC_OK) {
        fprintf(stderr, "Failed to run script \"%s\".\n", argv[1]);
        mc_destroy();
        return 1;
    }
    printf("+-------------\n");

    mc_destroy();

    return 0;
}
