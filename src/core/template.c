#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mycroft/template.h>

int mycroft_parse_template_str(template_root_t* root, char* template_str) {
    printf("THIS IS TEMPART %s", template_str);
}

int mycroft_parse_template_file(template_root_t* root, char* template_file) {

    /* Open file */
    FILE* f = fopen(template_file, "rb");
    if (f == NULL) {
        perror("Couldn't open template file");
        return 1;
    }

    /* Determine file size */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Allocate space to read file into */
    char* bytes = malloc(size);
    if (bytes == NULL) {
        perror("Couldn't malloc space to read template file");
        return 1;
    }

    /* Read file bytes */
    fread(bytes, size, 1, f);

    /* Parse template file */
    int res = mycroft_parse_template_str(root, bytes);

    /* Clean up and jet */
    free(bytes);
    return res;
}
