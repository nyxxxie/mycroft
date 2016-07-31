/**
 * @file main.c
 * @author nyx
 * @date 05/28/16
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <signal.h>
#include <mycroft/mycroft.h>
#include <mycroft/plugin.h>
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
uint64_t curaddr = 0;

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

void strip_newline(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

/** Command processer */
int process_command_args(char* argv[], int argc) {

    char* cmd = NULL;
    int rc = 0;
    int i = 0;

    if (argc <= 0) {
        return 0;
    }

    cmd = argv[0];
    if (strcmp("echo", cmd) == 0) {
        for (i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
    else if (strcmp("quit", cmd) == 0) {
        rc = 2;
    }
    else if (strcmp("runscript", argv[0]) == 0) {
        if (argc < 2) {
            printf("runscript - insufficient arguments (expected 2, got %i)\n", argc);
        }
    }
    else if (strcmp("runfile", argv[0]) == 0) {
        if (argc < 2) {
            printf("runfile - insufficient arguments (expected 2, got %i)\n", argc);
        }
        rc = mc_plugin_runfile(argv[1]);
    }
    else {
        printf("Unknown command: \"%s\"\n", cmd);
        rc = 1;
    }

    return rc;
}

/** Command processer */
int process_command(char* command, size_t bytes) {

    char* token = NULL;
    char** argv = NULL;
    int argc = 0;
    int rc = 0;
    int i = 0;

    token = strtok(command, " ,-");

    //TODO: process raw string commands here (eg: runstring)

    for (argc=0; token != NULL; token=strtok(NULL, " ,-")) {
        argc++;
        argv = realloc(argv, sizeof(char*)*argc);
        argv[argc-1] = malloc(strlen(token)+1);
        strcpy(argv[argc-1], token);
    }

    rc = process_command_args(argv, argc);

    for (i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);

    return rc;
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
        fprintf(stderr, "Failed to8 create template struct!\n");
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

        int rc = 0;
        size_t bytes = 100;
        char* input = NULL;

        /* Print input prompt */
        printf("[0x%08" PRIx64 "] :: ", curaddr);

        /* Take input */
        input = (char*)malloc(bytes + 1);
        getline(&input, &bytes, stdin);
        strip_newline(input);

        /* Process command */
        rc = process_command(input, bytes);
        free(input);

        if (rc == 2) {
            break;
        }
    }

    /* Cleanup */
    cleanup();

    return 0;
}
