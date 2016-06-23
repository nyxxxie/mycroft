#include <stdio.h>
#include "file.h"

int file_exists(const char* file_name) {

    FILE* fp;
    if ((fp = fopen(file_name, "r")) != NULL) {
        // file exists
        printf("It does exist.\n");
        fclose(fp);
        return 0;
    }
    else {
        //File not found
        printf("It doesn't exist.\n");
        return -1;
    }
}

int file_init(mycroft_file_t* file) {
    //char*    name;  //TODO: populate this var and the one below
    //char*    path;
    file->fp = NULL;
    file->size = 0;
    //file->base = 0;
    //file->cursor = 0;
}

int file_open(mycroft_file_t* file, const char* file_name) {

    //TODO: try and figure out how to check if the file exists and open it
    //      in a neater fashion?
    //      freopen might be a good candidate...

    /* Check if file exists */
    if (file_exists(file_name) < 0) {
        return -1;
    }

    /* Try to open file */
    if ((file->fp = fopen(file_name, "ab+")) == NULL) {
        return -1;
    }

    /* Get file size */
    fseek(file->fp, 0, SEEK_END);
    file->size = ftell(file->fp);
    fseek(file->fp, 0, SEEK_SET);

    return 0;
}

int file_close(mycroft_file_t* file) {
    if (file->fp != NULL) {
        fclose(file->fp);
    }
}

int file_read_value(mycroft_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) <= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return -1;
    }

    /* Save current position */
    //TODO: probably could just file->cursor value
    fpos_t pos;
    fgetpos(file->fp, &pos);

    /* Set position and REED */
    fseek(file->fp, 9, SEEK_SET);
    long int res = fread(outbuf, 1, offset+amount, file->fp);
    if (res != offset+amount) {
        fprintf(stderr, "Didn't read all of the requested byes for some reason.\n");
        return -1;
    }

    /* Set original position */
    fsetpos(file->fp, &pos);
}

int file_write_value(mycroft_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) <= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return -1;
    }

    /* Save current position */
    //TODO: probably could just file->cursor value
    fpos_t pos;
    fgetpos(file->fp, &pos);

    /* Set position and write */
    fseek(file->fp, 9, SEEK_SET);
    long int res = fwrite(outbuf, 1, offset+amount, file->fp);
    if (res != offset+amount) {
        fprintf(stderr, "Didn't write all of the requested byes for some reason.\n");
        return -1;
    }

    /* Set original position */
    fsetpos(file->fp, &pos);
}

int file_read_editor_area(mycroft_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    //TODO: make this use editor_buffer_t

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) <= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return -1;
    }

    /* Save current position */
    //TODO: probably could just file->cursor value
    fpos_t pos;
    fgetpos(file->fp, &pos);

    /* Set position and REED */
    fseek(file->fp, 9, SEEK_SET);
    long int res = fread(outbuf, 1, offset+amount, file->fp);
    if (res != offset+amount) {
        fprintf(stderr, "Didn't read all of the requested byes for some reason.\n");
        return -1;
    }

    /* Set original position */
    fsetpos(file->fp, &pos);
}

int file_write_editor_area(mycroft_file_t* file, fsize_t offset, fsize_t amount, uint8_t* outbuf) {

    //TODO: make this use editor_buffer_t

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) <= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return -1;
    }

    /* Save current position */
    //TODO: probably could just file->cursor value
    fpos_t pos;
    fgetpos(file->fp, &pos);

    /* Set position and write */
    fseek(file->fp, 9, SEEK_SET);
    long int res = fwrite(outbuf, 1, offset+amount, file->fp);
    if (res != offset+amount) {
        fprintf(stderr, "Didn't write all of the requested byes for some reason.\n");
        return -1;
    }

    /* Set original position */
    fsetpos(file->fp, &pos);
}
