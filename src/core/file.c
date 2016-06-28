#include <stdio.h>
#include <string.h>
#include "file.h"

int file_exists(const char* file_name) {

    FILE* fp;
    if ((fp = fopen(file_name, "r")) != NULL) {
        fclose(fp);
        return 0; //File exists
    }
    else {
        return -1; //File not found
    }
}

int file_init(mc_file_t* file, const char* file_name) {

    file->fp = NULL;
    file->size = 0;
    file->cursor = 0;

    file->path = (char*)malloc(strlen(file_name));
    if (file->path == NULL) {
        return -1;
    }
    strcpy(file->path, file_name);

    file->name = file->path;  //TODO: figure out how to get basename

    return 0;
}

int file_open(mc_file_t* file, const char* file_name) {

    if (file_name == 0 || strlen(file_name) == 0) {
        return -1;
    }

    if (file_init(file, file_name) < 0) {
        return -1;
    }

    /* Try to open file */
    if ((file->fp = fopen(file->path, "rb+")) == NULL) {
        return -1;
    }

    /* Copy file name and path */
    int nsize = strlen(file_name);
    file->path = (char*)malloc(nsize + 1);
    strcpy(file->path, file_name);
    file->name = basename(file->path);

    /* Get file size */
    fseek(file->fp, 0, SEEK_END);
    file->size = ftell(file->fp);
    fseek(file->fp, 0, SEEK_SET);

    return 0;
}

int file_close(mc_file_t* file) {
    if (file->fp != NULL) {
        fclose(file->fp);
    }

    return 0;
}

int file_get_cursor(mc_file_t* file) {
    //file->cursor = ftell(file->fp);
    return file->cursor;
}

int file_set_cursor(mc_file_t* file, int cursor) {

    if (cursor > file->size) {
        fprintf(stderr, "Tried to set cursor past the end of the file.");
        return -1;
    }

    int ret = fseek(file->fp, cursor, SEEK_SET);
    if (ret < 0) {
        perror("DEBUG: Error in file_set_cursor: ");
        return -1;
    }

    file->cursor = cursor;

    return 0;
}

fsize_t file_size(mc_file_t* file) {
    return file->size;
}

char* file_name(mc_file_t* file) {
    return file->name;
}

int file_read_pos(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) >= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return -1;
    }

    //TODO: check to see if we can read from editor_buffer's buf?

    /* Save old position and move to offset */
    fsize_t old_cursor = file->cursor;
    if (file_set_cursor(file, offset) < 0) {
        perror("DEBUG: Error in file_set_cursor: ");
        return -1;
    }

    /* Read desired content */
    long int res = fread(outbuf, 1, amount, file->fp);
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in file_write_pos");
            return -1;
        }
        else if (feof(file->fp)) {
            fprintf(stderr, "Tried to read past end of file.\n");
            return -1;
        }
        else {
            fprintf(stderr, "fread failed for some unknown reason.  Report this!\n");
            return -1;
        }
    }

    /* Set original position */
    if (file_set_cursor(file, old_cursor) < 0) {
        perror("DEBUG: Error in file_set_cursor: ");
        return -1;
    }

    return 0;
}

int file_write_pos(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    /* Make sure that we don't write past file contents */
    if ((offset + amount) >= file->size) {
        fprintf(stderr, "Tried to write off the end of the file.\n");
        return -1;
    }

    /* Save old position and move to offset */
    fsize_t old_cursor = file->cursor;
    if (file_set_cursor(file, offset) < 0) {
        return -1;
    }

    printf("cursor PRE = %i\n", ftell(file->fp));

    /* Read desired content */
    long int res = fwrite(outbuf, 1, amount, file->fp);
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in file_write_pos");
            return -1;
        }
        else if (feof(file->fp)) {
            // Reached eof (HOW??)
            return -1;
        }
        else {
            fprintf(stderr, "fwrite failed for some unknown reason.  Report this!\n");
            return -1;
        }
    }

    printf("cursor PST = %i\n", ftell(file->fp));

    /* Set original position */
    if (file_set_cursor(file, old_cursor) < 0) {
        perror("DEBUG: Error in file_set_cursor");
        return -1;
    }

    return 0;
}

//TODO: implement the rest of these functions
