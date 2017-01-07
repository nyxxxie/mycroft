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
