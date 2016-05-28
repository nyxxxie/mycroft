/**
 * @file main.c
 * @author nyx
 * @date 05/28/16
 */

#include <stdio.h>
#include <argp.h>

#include "config.h"

 /* Argp strings */
const char* argp_program_version = MYCROFTD_VERSION;
const char* argp_program_bug_address = "nyxxie at protonmail.ch";
static char doc[] = "Linux-based reverse engineering tool.";

/* Argp options */
static struct argp_option options[] = {
    {"file", 'f', "FILE", 0, "File to play with."},
    {0}
};

/* Arguement values to pass to main func */
typedef struct {
    char* file;
} arguments_t;

/* Option parser */
static error_t parse_opt(int key, char* arg, struct argp_state* state) {
    /* Get the input arguement from argp_parse, which we know is a pointer
     * to our arguments structure. */
    arguments_t* arguments = (arguments_t*)state->input;

    switch (key) {
    case 'f':
        arguments->file = arg;
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, 0, doc};

/* Main function */
int main(int argc, char *argv[]) {

    /* Create args */
    arguments_t arguments;
    arguments.file = 0;

    /* Parse arguments */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Print file name for testing! */
    if (arguments.file != 0) {
        printf("FILE %s\n", arguments.file);
    }

    printf("DO SHIT\n");

    return 0;
}
