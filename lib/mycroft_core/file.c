/**
 * @file
 * @brief Provides an interface to file IO for mycroft.
 *
 * This abstraction is necessary so that we can perform operations on big files
 * across systems.  It also allows us to make file operations more neat and
 * modular.  For example, a client can edit multiple files by simply
 * initializing and opening files on two mc_file_t structs and then simply
 * switching which pointer it uses.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <mycroft/file.h>
#include <mycroft/error.h>
#include "file.h"

/**
 * Handy utility function for determining whether or not a given file exists.
 * Please note that using this function before mc_file_open is unnecessary;
 * mc_file_open already performs this check internally.
 *
 * @param file_name File to check.
 *
 * @return Returns 0 if file exists, other value otherwise.
 */
/* NOTE: return type is subject to change */
int mc_file_access(const char* file_name)
{
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
 * Initializes an mc_file_t struct.  It is REQURIED that this function is
 * called prior to using any other function that takes the same mc_file_t
 * struct as an* argument.  Failure to do so will yeild undefined behavior.
 *
 * @return Returns the created mc_file_t, NULL otherwise.
 */
mc_file_t* mc_file_create()
{
    mc_file_t* file = NULL;

    /* Alloc the project */
    file = (mc_file_t*)malloc(sizeof(mc_file_t));
    if (file == NULL) {
        return NULL;
    }

    file->fp = NULL;
    file->size = 0;
    file->cursor = 0;
    file->name = NULL;
    file->path = NULL;
    file->t = NULL;
    file->cache = NULL;

    return file;
}

/**
 * Opens a file.  Make sure you call mc_file_close after you're done with the
 * file you've opened.  Do not call this function twice on the same struct
 * without closing and reinitializing it.
 *
 * @param file_name File to open.
 * @param err Pointer to mc_error_t to return error to.  May be NULL.
 *
 * @return Returns the created mc_file_t, NULL otherwise.
 */
mc_file_t* mc_file_open(const char* file_name, mc_error_t *err)
{
    mc_file_t* file = NULL;

    /* Make sure we're passed a nonempty path */
    if (file_name == 0 || strlen(file_name) == 0) {
        MC_ERROR("Filename is null or empty.");
        if (err != NULL)
            (*err) = MC_FILE_ERR_FILENAME;
        return NULL;
    }

    /* Create file */
    file = mc_file_create();
    if (file == NULL) {
        MC_ERROR("Failed to alloc mc_file_t struct.");
        if (err != NULL)
            (*err) = MC_FILE_ERR_MALLOC;
        return NULL;
    }

    /* Fill the file name and path members of the mc_file_t struct */
    file->path = (char*)malloc(strlen(file_name)+1);
    if (file->path == NULL) {
        MC_ERROR("Failed to copy file path to mc_file_t struct.");
        if (err != NULL)
            (*err) = MC_FILE_ERR_MALLOC;
        return NULL;
    }
    strcpy(file->path, file_name);
    file->name = basename(file->path);

    /* Try to open file */
    if ((file->fp = fopen(file->path, "rb+")) == NULL) {
        MC_ERROR("Failed to fopen file \"%s\": %s [%i]\n",
                 file->path, strerror(errno), errno);
        if (err != NULL)
            (*err) = MC_FILE_ERR_IO;
        return NULL;
    }

    /* Get file size */
    fseeko(file->fp, 0, SEEK_END);
    file->size = ftello(file->fp);
    fseeko(file->fp, 0, SEEK_SET);

    return file;
}

/**
 * Closes a file.  Make sure to call this when you're done with a file
 * otherwise you'll leak memory and orphan a FILE* pointer.
 *
 * @param file mc_file_t struct to close.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_close(mc_file_t* file)
{
    if (file->fp != NULL)
        fclose(file->fp);
    if (file->path != NULL)
        free(file->path);

    return MC_OK;
}

/**
 * Get template associated with this file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the template or NULL if it's not set.
 */
template_t* mc_file_get_template(mc_file_t* file)
{
    return file->t;
}

/**
 * Set template to be associated with this file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_set_template(mc_file_t* file, template_t* t)
{
    file->t = t;
    return MC_OK;
}

/**
 * Get file custor position.  This is where the file will be reading/writing
 * from.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns cursor.
 */
fsize_t mc_file_get_cursor(mc_file_t* file)
{
    //file->cursor = ftello(file->fp);
    return (fsize_t) file->cursor;
}

/**
 * Set file cursor position.  This will determine where we start
 * reading/writing from.
 *
 * @param file mc_file_t struct to operate on.
 * @param cursor Value to set cursor to.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_set_cursor(mc_file_t* file, fsize_t cursor)
{
    if (cursor > file->size) {
        fprintf(stderr, "Tried to set cursor past the end of the file.");
        return MC_FILE_ERR_BOUNDS;
    }
    int ret = fseeko(file->fp, (off_t) cursor, SEEK_SET);
    if (ret != 0) {
        perror("DEBUG: Error in mc_file_set_cursor: ");
        return MC_FILE_ERR_IO;
    }
    file->cursor = cursor;
    return MC_OK;
}

/**
 * Returns the size of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file size.
 */
fsize_t mc_file_size(mc_file_t* file)
{
    return file->size;
}

/**
 * Returns the name of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file name.
 */
char* mc_file_name(mc_file_t* file)
{
    return file->name;
}

/**
 * Returns the path of the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns the file path.
 */
char* mc_file_path(mc_file_t* file)
{
    return file->path;
}

/**
 * This function must be called for the cache to be used.  This will allocate a
 * cache space of size <size> and preload the first <size> bytes of the file
 * into it.  Cache is useful to reduce disk IO and speed up file operations.
 * Please note that cache does not currently provide any real performance
 * benifit to write operations.
 *
 * @param file mc_file_t struct to operate on.
 * @param size Size of the cache to allocate.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_cache_init(mc_file_t* file, fsize_t size)
{
    /* Create cache struct */
    file->cache = (file_cache_t*)malloc(sizeof(file_cache_t));
    if (file->cache == NULL) {
        fprintf(stderr, "Failed to allocate space for cache struct.");
        return MC_FILE_ERR_MALLOC;
    }

    /* Determine if the cache is larger than the file, and resize it accordingly */
    //if (size > mc_file_size(file))
    //    size = mc_file_size(file); // TODO: make this a setting?

    file->cache->size = size;
    file->cache->base = 0;

    /* Create the cache buffer */
    file->cache->buf = (uint8_t*)malloc(file->cache->size);
    if (file->cache->buf == NULL) {
        fprintf(stderr, "Failed to allocate space for cache buffer.");
        return MC_FILE_ERR_MALLOC;
    }

    return MC_OK;
}

/**
 * Loads the bytes starting at <cursor> into the cache.
 *
 * @param file mc_file_t struct to operate on.
 * @param cursor Position to start at.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_cache_loadzone(mc_file_t* file, fsize_t cursor)
{
    file->cache->base = cursor;
    /* This will then load data from file from a newly set cursor */
    return mc_file_cache_reload(file);
}

/**
 * Re-reads the current cache contents from the file.
 *
 * @param file mc_file_t struct to operate on.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_cache_reload(mc_file_t* file)
{
    /* If our cache extends off the end of the current file, relocate it */
    fsize_t diff = (file->cache->base+file->cache->size) - mc_file_size(file);
    if (diff > 0)
        file->cache->base -= diff;
    /* Read bytes from file into cache */
    mc_error_t rc = mc_file_read_raw(file, file->cache->size,
                                     file->cache->buf, NULL);
    if (rc != MC_OK)
        return rc;
    return MC_OK;
}

/**
 * Reads bytes from a file.  Will move the cursor to the end of the file.
 * This function is suitable for reading large amount of bytes.
 *
 * @param file mc_file_t struct to operate on.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 * @param amount_read Amount of bytes read.  Can be NULL.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_read(mc_file_t* file, fsize_t amount,
                        uint8_t* outbuf, fsize_t* amount_read)
{
    /* Read desired content */
    size_t res = fread(outbuf, 1, amount, file->fp);
    if (amount_read != NULL)
        (*amount_read) = (fsize_t) res;
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in mc_file_read");
            return MC_FILE_ERR_IO;
        }
    }
    return MC_OK;
}

/**
 * Reads bytes from a file.  Will move the cursor to the end of the file.
 * This function is suitable for reading large amount of bytes.
 *
 * @param file mc_file_t struct to operate on.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 * @param amount_read Amount of bytes read.  Can be NULL.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_read_raw(mc_file_t* file, fsize_t amount,
                            uint8_t* outbuf, fsize_t* amount_read)
{
    /* Read desired content */
    size_t res = fread(outbuf, 1, amount, file->fp);
    if (amount_read != NULL)
        (*amount_read) = (fsize_t) res;
    if (res != amount) {
        if (ferror(file->fp) != 0) {
            perror("DEBUG: Error in mc_file_read_raw");
            return MC_FILE_ERR_IO;
        }
    }
    return MC_OK;
}

/**
 * Tries to read a value in the file.  This function is meant to be used to
 * read a single value or struct from a file.  If you want to read a lot of
 * bytes, use a combination of mc_file_read and mc_file_set_cursor.  This
 * function doesn't change the cursor position.
 *
 * @param file mc_file_t struct to operate on.
 * @param offset Offset into the file to start reading at.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 * @param amount_read Amount of bytes read.  Can be NULL.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_read_value(mc_file_t* file, fsize_t offset,
                              fsize_t amount, uint8_t* outbuf,
                              fsize_t* amount_read)
{
    mc_error_t rc;

    /* Make sure that we don't read off the end of the file */
    if ((offset + amount) >= file->size) {
        fprintf(stderr, "Tried to read off the end of the file.\n");
        return MC_FILE_ERR_BOUNDS;
    }

    //TODO: check to see if we can read from editor_buffer's buf?

    /* Save old position and move to offset */
    fsize_t old_cursor = file->cursor;
    rc = mc_file_set_cursor(file, offset);
    if (rc != MC_OK) {
        perror("DEBUG: Error in mc_file_set_cursor: ");
        return rc;
    }

    /* Read desired content */
    size_t res = fread(outbuf, 1, amount, file->fp);
    if (amount_read != NULL)
        (*amount_read) = (fsize_t) res;
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in mc_file_write_pos");
            return MC_FILE_ERR_IO;
        }
        else if (feof(file->fp)) {
            fprintf(stderr, "Tried to read past end of file.\n");
            return MC_FILE_ERR_IO;
        }
        else {
            fprintf(stderr,
                    "fread failed for some unknown reason.  "
                    "Report this!\n");
            return MC_ERR;
        }
    }

    /* Set original position */
    rc = mc_file_set_cursor(file, old_cursor);
    if (rc != MC_OK) {
        perror("DEBUG: Error in mc_file_set_cursor: ");
        return rc;
    }

    return MC_OK;
}

/**
 * Writes bytes to a file.  Will move the cursor to the end of the file.
 * This function is suitable for writing large amount of bytes.
 *
 * @param file mc_file_t struct to operate on.
 * @param amount Amount of bytes to write.
 * @param outbuf Buffer to place bytes in.
 * @param amount_written Amount of bytes written.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_write(mc_file_t* file, fsize_t amount,
                         uint8_t* outbuf, fsize_t* amount_written)
{
    /* Read desired content */
    size_t res = fwrite(outbuf, 1, amount, file->fp);
    if (amount_written != NULL)
        (*amount_written) = (fsize_t) res;
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in mc_file_write");
            return MC_FILE_ERR_IO;
        }
    }
    return MC_OK;
}

/**
 * Tries to write a value to the file.  This function is meant to be used to
 * write a single value or struct to a file.  If you want to read a lot of
 * bytes, use a combination of mc_file_read and mc_file_set_cursor.  This
 * function doesn't change the cursor position.  This will overwite whatever
 * value is at the offset position.
 *
 * @param file mc_file_t struct to operate on.
 * @param offset Offset into the file to start reading at.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 * @param amount_written Amount of bytes written.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_write_value(mc_file_t* file, fsize_t offset,
                               fsize_t amount, uint8_t* outbuf,
                               fsize_t* amount_written)
{
    mc_error_t rc;

    /* Make sure that we don't write past file contents */
    if ((offset + amount) >= file->size) {
        fprintf(stderr, "Tried to write off the end of the file.\n");
        return MC_FILE_ERR_BOUNDS;
    }

    /* Save old position and move to offset */
    fsize_t old_cursor = file->cursor;
    rc = mc_file_set_cursor(file, offset);
    if (rc != MC_OK) {
        perror("DEBUG: Error in mc_file_set_cursor: ");
        return rc;
    }

    /* Read desired content */
    size_t res = fwrite(outbuf, 1, amount, file->fp);
    if (amount_written != NULL)
        (*amount_written) = (fsize_t) res;
    if (res != amount) {
        if (ferror(file->fp)) {
            perror("DEBUG: Error in mc_file_write_pos");
            return MC_FILE_ERR_IO;
        }
        else {
            fprintf(stderr,
                    "fwrite failed for some unknown reason.  "
                    "Report this!\n");
            return MC_ERR;
        }
    }

    /* Set original position */
    rc = mc_file_set_cursor(file, old_cursor);
    if (rc != MC_OK) {
        perror("DEBUG: Error in mc_file_set_cursor");
        return rc;
    }

    return MC_OK;
}

/**
 * Reads bytes from the file into the cache.  This function should be used with
 * caution, as it influences the position of the cache and could result in a
 * LOT of disk usage if not used properly.  For instance, if your cache size is
 * huge and you use this function to read a value at the end of the current
 * cache location, you'll cause to cache to move and thus reload all of that
 * space.
 *
 * @param file mc_file_t struct to operate on.
 * @param offset Offset into the file to start reading at.
 * @param amount Amount of bytes to read.
 * @param outbuf Buffer to place bytes in.
 * @param amount_written Amount of bytes read.
 *
 * @return Returns mc_error_t status.
 */
mc_error_t mc_file_read_cache(mc_file_t* file, fsize_t offset,
                              fsize_t amount, uint8_t* outbuf,
                              fsize_t* amount_read)
{
    mc_error_t rc;

    rc = mc_file_cache_loadzone(file, offset);
    if (rc != MC_OK)
        return rc;

    rc = mc_file_set_cursor(file, offset);
    if (rc < 0)
        return rc;

    return mc_file_read(file, amount, outbuf, amount_read);
}
