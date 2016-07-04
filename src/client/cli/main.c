/**
 * @file main.c
 * @author nyx
 * @date 05/28/16
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <argp.h>
#include <signal.h>
#include <mycroft/mycroft.h>
#include "config.h"

/** Argp strings */
const char* argp_program_version = MYCROFTD_VERSION;
const char* argp_program_bug_address = "nyxxie at protonmail.ch";
static char doc[] = "Linux-based reverse engineering tool.";
static char args_doc[] = "filename";

/** Argp options */
static struct argp_option options[] = {
    {"file", 'f', "FILE", 0, "File to play with."},
    {"config", 'c', "FILE", 0, "Config file to load settings from."},
    {0}
};

/** Arguement values to pass to main func */
typedef struct {
    int   arg_num;
    char* target_file;
    char* config_file;
} arguments_t;

/** Option parser */
static error_t parse_opt(int key, char* arg, struct argp_state* state) {
    /* Get the input arguement from argp_parse, which we know is a pointer
     * to our arguments structure. */
    arguments_t* arguments = (arguments_t*)state->input;

    switch (key) {
    case 'f':
        arguments->target_file = arg;
        break;

    case 'c':
        arguments->config_file = arg;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            argp_usage (state);
        else
            arguments->target_file = arg;

        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

/* Main function */
char shellcode[] = {
    "\x6a\x0b"                      /* push   0xb        */
    "\x58"                          /* pop    eax        */
    "\x31\xf6"                      /* xor    esi,esi    */
    "\x56"                          /* push   esi        */
    "\x68\x2f\x2f\x73\x68"          /* push   0x68732f2f */
    "\x68\x2f\x62\x69\x6e"          /* push   0x6e69622f */
    "\x89\xe3"                      /* mov    ebx,esp    */
    "\x31\xc9"                      /* xor    ecx,ecx    */
    "\x89\xca"                      /* mov    edx,ecx    */
    "\xcd\x80"                      /* int    0x80       */
};

static mc_ctx_t* ctx;

/** Cleanup function */
void cleanup() {
    mycroft_free(ctx);
    printf("Cleaned up successfully.\n");
}

/** Signal handler */
void signal_callback_handler(int signum) {
   printf("Caught signal %d\n",signum);
   cleanup();
   exit(signum);
}

/** Command processer */
int process_command(char* command, char* argv[], int argc) {

}

/** Entry point */
int main(int argc, char *argv[]) {

    /* Create args */
    arguments_t arguments;
    arguments.arg_num = 0;
    arguments.target_file = '\0';
    arguments.config_file = '\0';

    /* Parse arguments */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Init signal handler */
    signal(SIGINT, signal_callback_handler);
    printf("Created signal handler...\n");

    /* Init mycroft core */
    ctx = mycroft_init();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create template struct!\n");
        exit(1);
    }
    printf("Initialized core...\n");

    /* Print file name for testing! */
    if (arguments.target_file != 0) {
        printf("FILE %s\n", arguments.target_file);
        mycroft_open_file(ctx, arguments.target_file);
    }
    else {
        printf("Please specify a file.\n");
        return 1;
    }
    printf("Opened file...\n");

    /* Main command loop */
    while (1) {

        /* Print input prompt */
        printf("[0x%08x] :: ");

        /* Take input */

        /* Process command */
        //process_command(cmd, args, argc);

        /* Process custom commands */
    }

    /* Cleanup */
    cleanup();

    return 0;
}
