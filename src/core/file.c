#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "file.h"

/**
 * Handy utility function for determining whether or not a given file exists.
 * Please note that using this function before file_open is unnecessary;
 * file_open already performs this check internally.
 *
 * @param file_name File to check.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_exists(const char* file_name) {

    FILE* fp;
    if ((fp = fopen(file_name, "r")) != NULL) {
        fclose(fp);
        return 0;
    }
    else {
        return -1;
    }
}

/**
 * Initializes an mc_file_t struct.  It is REQURIED that this function is called
 * prior to using any other function that takes the same mc_file_t struct as an
 * argument.  Failure to do so will yeild undefined behavior.
 *
 * @param file mc_file_t struct to initialize.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_init(mc_file_t* file) {

    file->fp = NULL;
    file->size = 0;
    file->cursor = 0;
    file->name = NULL;
    file->path = NULL;
    return 0;
}

/**
 * Opens a file.  Make sure you call file_init before using this function and
 * call file_close after you're done with the file you've opened.  Do not call
 * this function twice on the same struct without closing and reinitializing it.
 *
 * @param file mc_file_t struct to operate on.
 * @param file_name File to open.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_open(mc_file_t* file, const char* file_name) {

    /* Make sure we're passed a nonempty path */
    if (file_name == 0 || strlen(file_name) == 0) {
        return -1;
    }

    /* Fill the file name and path members of the mc_file_t struct */
    file->path = (char*)malloc(strlen(file_name));
    if (file->path == NULL) {
        return -1;
    }
    strcpy(file->path, file_name);
    file->name = file->path;  //TODO: figure out how to get basename

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

/**
 * Closes a file.  Make sure to call this when you're done with a file
 * otherwise you'll leak memory and orphan a FILE* pointer.
 *
 * @param file mc_file_t struct to close.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_close(mc_file_t* file) {
    if (file->fp != NULL) {
        fclose(file->fp);
    }

    if (file->path != NULL) {
        free(file->path);
    }

    return 0;
}

/**
 * Get file custor position.  This is where the file will be reading/writing
 * from.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_get_cursor(mc_file_t* file) {
    //file->cursor = ftell(file->fp);
    return file->cursor;
}

/**
 * Set file cursor position.  This will determine where we start
 * reading/writing from.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns 0 on success, negative value on error.
 */
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

/**
 * Returns the size of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file size.
 */
fsize_t file_size(mc_file_t* file) {
    return file->size;
}

/**
 * Returns the name of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file name.
 */
char* file_name(mc_file_t* file) {
    return file->name;
}

/**
 * Returns the path of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file path.
 */
char* file_path(mc_file_t* file) {
    return file->path;
}

/**
 * Reads bytes from a file.  Will move the cursor to the end of the file.
 * This function is suitable for reading large amount of bytes.
 *
 * @param file mc_file_t struct to operate on.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_read(mc_file_t* file, fsize_t amount, uint8_t* outbuf) {

    /* Read desired content */
    long int res = fread(outbuf, 1, amount, file->fp);
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in file_write_pos");
            return -1;
        }
    }

    return res;
}

/**
 * Tries to read a value in the file.  This function is meant to be used to
 * read a single value or struct from a file.  If you want to read a lot of
 * bytes, use a combination of file_read and file_set_cursor.  This function
 * doesn't change the cursor position.
 *
 * @param file mc_file_t struct to operate on.
 * @param offset Offset into the file to start reading at.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_read_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

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

/**
 * Writes bytes to a file.  Will move the cursor to the end of the file.
 * This function is suitable for writing large amount of bytes.
 *
 * @param file mc_file_t struct to operate on.
 * @param amount Amount of bytes to write.
 * @param outbuf Buffer to place bytes in.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_write(mc_file_t* file, fsize_t amount, uint8_t* outbuf) {

}

/**
 * Tries to write a value to the file.  This function is meant to be used to
 * write a single value or struct to a file.  If you want to read a lot of
 * bytes, use a combination of file_read and file_set_cursor.  This function
 * doesn't change the cursor position.  This will overwite whatever value is
 * at the offset position.
 *
 * @param file mc_file_t struct to operate on.
 * @param offset Offset into the file to start reading at.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 *
 * @return Returns 0 on success, negative value on error.
 */
int file_write_value(mc_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

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
