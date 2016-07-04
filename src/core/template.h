#ifndef MYCROFT_TEMPLATE_H
#define MYCROFT_TEMPLATE_H

#include <stdint.h>
#include "core.h"

#define MC_TEMPLATE_ENTRY  0
#define MC_TEMPLATE_STRUCT 1

#define MC_TEMPLATE_ENAME_MAX_SIZE 255
#define MC_TEMPLATE_SNAME_MAX_SIZE 255
#define MC_TEMPLATE_TNAME_MAX_SIZE 64
#define MC_TEMPLATE_COMMENT_MAX_SIZE 255

#ifdef __cplusplus
extern "C" {
#endif

/** Generic entry.  Only really useful for accessing the type specifier. */
typedef struct {
    uint8_t  type;
} mc_template_entry_t;

/** Represents a variable declaration in a struct */
typedef struct {
    uint8_t  type;
    char     name[MC_TEMPLATE_ENAME_MAX_SIZE];
    uint32_t start_offset;
    char     type_name[MC_TEMPLATE_TNAME_MAX_SIZE];
    uint32_t type_size;;
    char     color_fg[4];
    char     color_bg[4];
    char     comment[MC_TEMPLATE_COMMENT_MAX_SIZE];
} mc_template_var_t;

/** Represents a struct declaration */
typedef struct {
    uint8_t           type;
    char              name[MC_TEMPLATE_SNAME_MAX_SIZE];
    int               entry_amount;
    mc_template_var_t* entries;
    char              color_fg[4];
    char              color_bg[4];
    char              comment[MC_TEMPLATE_COMMENT_MAX_SIZE];
} mc_template_struct_t;

/** Root node in a template tree */
typedef struct {
    template_entry_t* root;
} mc_template_t;

/* Init/Exit */
mc_template_t* mycroft_template_init();
void mycroft_template_free(mc_template_t* t);

/* Template loading/creation */
void mycroft_template_create(mc_template_t* t);
int  mycroft_template_parse_str(mc_template_t* t, char* template_str);
int  mycroft_template_parse_file(mc_template_t* t, mc_file_t* file);

#ifdef __cplusplus
}
#endif

#endif // TEMPLATE_H
