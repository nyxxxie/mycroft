#include <stdio.h>
#include "config.h"
#include <mycroft/context.h>
#include <mycroft/plugin.h>
#include <mycroft/script.h>

int main(int argc, char* argv[])
{
    mc_ctx_t* ctx = NULL;
    mc_project_t* project = NULL;

    /* Checked to see if a file was specified */
    if (argc < 1) {
        printf("Script file not specified.\n");
        return 1;
    }

    /* Create context */
    ctx = mc_ctx_create();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create ctx, exiting...\n");
        return 1;
    }

    /* Create project */
    project = mc_project_create("test project");
    if (project == NULL) {
        fprintf(stderr, "Failed to create project, exiting...\n");
        return 1;
    }

    if (mc_script_runfile(ctx, argv[1]) != MC_OK) {
        fprintf(stderr, "Failed to run script \"%s\".\n", argv[1]);
        return 1;
    }

    printf("Ran script: \"%s\"\n", argv[1]);

    return 0;
}
