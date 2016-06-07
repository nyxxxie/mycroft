/**
 * @file main.c
 * @author nyx
 * @date 05/28/16
 */

#include <stdio.h>
#include <argp.h>
#include <stdio.h>
#include <stdint.h>

#include <capstone/capstone.h>

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
char shellcode[] = {
    "\x6a\x0b"                      /* push   0xb */
    "\x58"                          /* pop    eax */
    "\x31\xf6"                      /* xor    esi,esi */
    "\x56"                          /* push   esi */
    "\x68\x2f\x2f\x73\x68"          /* push   0x68732f2f */
    "\x68\x2f\x62\x69\x6e"          /* push   0x6e69622f */
    "\x89\xe3"                      /* mov    ebx,esp */
    "\x31\xc9"                      /* xor    ecx,ecx */
    "\x89\xca"                      /* mov    edx,ecx */
    "\xcd\x80"                      /* int    0x80 */
};
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





    /* ------------ Shit ------------ */
    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
        return -1;

    cs_insn* insn;
    size_t count = cs_disasm(handle,
        shellcode, sizeof(shellcode)-1,
        0x1000, 0, &insn);

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            printf("0x%x:\t%s\t%s\n", insn[i].address, insn[i].mnemonic,
            insn[i].op_str);
        }

        cs_free(insn, count);
    } else
        printf("ERROR: Failed to disassemble given code!\n");

    cs_close(&handle);

    return 0;
}
