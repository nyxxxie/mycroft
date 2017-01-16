#include "gtest/gtest.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "typesystem/typesystem.h"

/* This function is overcomplicated because I was using it to test an idea for
   converter that would be able to operate on integers of an arbitrary size.
   To do this, I planned on performing the conversion by removing the unpacking
   code which uses the builtin int size as a convenient container and replacing
   the mathematical operations that operate on input with equivilents that use 
   bitwise operations and operate directly on the bits in the byte buffer.
   This is going to be scrapped for now because it'd take too much time to
   write for no real benifit over just implementing the type in python, where
   ints can be n big */
mc_error_t to_string(mc_typedef_t* type, uint8_t* bytes, size_t byte_amount, char** strbuf, size_t* strbuf_len)
{
    int neg = 0;
    int tmp = 0;
    int size = 1;
    int input = 0;

    /* Check to make sure input is valid */
    if (byte_amount != 4 || bytes == NULL) {
        return MC_ERR;
    }

    /* Use the built-in integer type for onvenience */
    input = 0;
    input |= bytes[0] << 24;
    input |= bytes[1] << 16;
    input |= bytes[2] << 8;
    input |= bytes[3];

    /* Check if number is negative */
    if (input < 0) {
        size++; // Add space for negative sign
        neg = 1;
        input = (~input) + 1; // Un-negate
    }

    /* Calc string size required to hold this integer */
    tmp = input;
    do {
        size++;
        tmp /= 10;
    } while (tmp);

    /* If we haven't been given a buffer that can hold this string, allocate one */
    if (*strbuf == NULL) {
        char* str;

        /* Alloc string buffer.  We don't assign this to *string directly 
           because there's a possibility this could fail and we don't want to
           modify the input variables if we have failed. */
        str = (char*)malloc(size);
        if (str == NULL) {
            MC_ERROR("Failed to allocate memory for strbuf.\n");
            return MC_ERR;
        }

        /* Give caller a useable (and freeable) buffer */
        *strbuf = str;
    }

    /* We've been given a buffer to use, is it too small? */
    else if (*strbuf_len < size) {
        MC_ERROR("Provided buffer was too small to fit integer.\n");
        return MC_ERR;
    }

    /* Inform caller of the size of the string buffer */
    *strbuf_len = size;

    /* Move str pointer to end of string buffer (subtract 1 for null term) */
    *strbuf += size - 1;

    /* Add null terminator */
    **strbuf = '\0';

    /* Get digit in the input number and add them to the buffer */
    do {
        *--*strbuf = (input % 10) + 0x30; // input % 10 gets the value of the most signifigant digit, + 0x30 is a trick to offset into the number column in the ascii table
        input /= 10;
    } while (input);

    /* Add negative sign if the number is negative */
    if (neg) {
        *--*strbuf = '-';
    }

    return MC_OK;
}

mc_error_t from_string(mc_typedef_t* type, const char* string, uint8_t** bytes, size_t* byte_amount)
{
    int value;

    /* Convert string to int the easy way */
    value = atoi(string);

    if (*bytes == NULL) {
        uint8_t* b;

        /* Alloc byte buffer.  We don't assign this to *string directly because
           there's a possibility this could fail and we don't want to modify 
           the input variables if we have failed. */
        b = (uint8_t*)malloc(4);
        if (b == NULL) {
            MC_ERROR("Failed to allocate buffer for bytes.\n");
            return MC_ERR;
        }

        *bytes = b;
    }
    else if (*byte_amount != 4) {
        MC_ERROR("Provided buffer was too small to fit bytes.\n");
        return MC_ERR;
    }

    *byte_amount = 4;

    /* Unpack bytes of integer value into byte buffer */
    (*bytes)[0] = (uint8_t)(value >> 24);
    (*bytes)[1] = (uint8_t)(value >> 16);
    (*bytes)[2] = (uint8_t)(value >> 8);
    (*bytes)[3] = (uint8_t)value;

    return MC_OK;
}

/**
 */
TEST(typesystem_created, create_typesystem)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);
    ASSERT_TRUE(t->typedefs == NULL);
    ASSERT_TRUE(t->typedef_amount == 0);

    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, create_type)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_to_string)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0x00, 0x00, 0xff, 0xff};
    size_t byte_amount = 4;
    char* string = NULL;
    size_t string_length = 0;
    ASSERT_TRUE(mc_typedef_to_string(type, bytes, byte_amount, &string, &string_length) == MC_OK);

    ASSERT_TRUE(string != NULL);
    ASSERT_TRUE(strlen(string) == 5);
    ASSERT_TRUE(strcmp(string, "65535") == 0);

    free(string);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_to_string2)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0xff, 0xff, 0xfa, 0xc7};
    size_t byte_amount = 4;
    char* string = NULL;
    size_t string_length = 0;
    ASSERT_TRUE(mc_typedef_to_string(type, bytes, byte_amount, &string, &string_length) == MC_OK);

    ASSERT_TRUE(string != NULL);
    ASSERT_TRUE(strlen(string) == 5);
    ASSERT_TRUE(strcmp(string, "-1337") == 0);

    free(string);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_to_string_preallocated)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0xff, 0xff, 0xfa, 0xc7};
    size_t byte_amount = 4;
    size_t string_length = 6;
    char* string = (char*)malloc(string_length);
    ASSERT_TRUE(mc_typedef_to_string(type, bytes, byte_amount, &string, &string_length) == MC_OK);

    ASSERT_TRUE(string != NULL);
    ASSERT_TRUE(strlen(string) == 5);
    ASSERT_TRUE(strcmp(string, "-1337") == 0);

    free(string);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_from_string)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0x00, 0x00, 0xff, 0xff};

    uint8_t* converted_bytes = NULL;
    size_t byte_amount = 0;
    ASSERT_TRUE(mc_typedef_from_string(type, "65535", &converted_bytes, &byte_amount) == MC_OK);

    for (int i = 0; i < byte_amount; i++) {
        if (bytes[i] != converted_bytes[i]) {
            ASSERT_TRUE(false);
        }
    }

    free(converted_bytes);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_from_string2)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0xff, 0xff, 0xfa, 0xc7};

    uint8_t* converted_bytes = NULL;
    size_t byte_amount = 0;
    ASSERT_TRUE(mc_typedef_from_string(type, "-1337", &converted_bytes, &byte_amount) == MC_OK);

    for (int i = 0; i < byte_amount; i++) {
        if (bytes[i] != converted_bytes[i]) {
            ASSERT_TRUE(false);
        }
    }

    free(converted_bytes);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, call_from_string_preallocated)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0xff, 0xff, 0xfa, 0xc7};

    uint8_t* converted_bytes = (uint8_t*)malloc(4);
    size_t byte_amount = 4;
    ASSERT_TRUE(mc_typedef_from_string(type, "-1337", &converted_bytes, &byte_amount) == MC_OK);

    for (int i = 0; i < byte_amount; i++) {
        if (bytes[i] != converted_bytes[i]) {
            ASSERT_TRUE(false);
        }
    }

    free(converted_bytes);
    mc_typesystem_free(t);
}

/**
 */
TEST(typesystem_created, to_string_bad_size)
{
    mc_typesystem_t* t = NULL;

    t = mc_typesystem_create();
    ASSERT_TRUE(t != NULL);

    ASSERT_TRUE(mc_typesystem_create_type(t, "what", 4, to_string, from_string) == MC_OK);

    mc_typedef_t* type = mc_typesystem_get_type(t, "what");
    ASSERT_TRUE(type != NULL);

    uint8_t bytes[] = {0xff, 0xff, 0xfa, 0xc7};
    size_t byte_amount = 4;
    size_t string_length = 2;
    char* string = (char*)malloc(string_length);
    ASSERT_TRUE(mc_typedef_to_string(type, bytes, byte_amount, &string, &string_length) != MC_OK);

    free(string);
    mc_typesystem_free(t);}

// TODO: write tests to test both the python and c type system.  Find a nice way to make byte and char builtins, where char displays ascii and byte takes 0x00 or BB values (or any way hex can be input)
