#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "template_parse.h"

mc_template_t* mycroft_template_init() {

}

void mycroft_template_free(mc_template_t* t) {

}

void mycroft_template_create(mc_template_t* t) {

}

int mycroft_template_parse_str(mc_template_t* t, char* template_str) {

}

int mycroft_template_parse_file(mc_template_t* t, mc_file_t* file) {

}


/**
 * Populates a template structure with the values loaded from a file.
 *
 * @param t mc_template_t structure to operate on.
 * @param file File to read values in from.
 *
 * @return Returns 0 on success, negative value on error.
 */
int mycroft_template_apply(mc_template_t* t, mc_file_t* file) {
    
}

// TODO: when we add event system, create a way to update only
//       values in areas where file editing took place.

// TODO: delete this after testing is done
int int main(int argc, char *argv[]) {

    /* Make sure we pass an argument */
    if (argc <= 1) {
        printf("Usage: %s <file>", argv[0]);
        return 1;
    }

    mc_file_t f;
    file_init(&f);
    if (file_open(&f, argv[1]) < 0) {
        printf("Failed to open file \"%s\".\n", argv[1]);
        return 1;
    }

    mc_template_t* template = mycroft_template_init();
    if (template == NULL) {
        fprintf(stderr, "Failed to allocate template.\n");
        return -1;
    }

    if (mycroft_template_parse_file()) {
        return -1;
    }

    return 0;
}
