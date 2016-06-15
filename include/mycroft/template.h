#include <stdint.h>

#define MC_TEMPLATE_ENTRY  0
#define MC_TEMPLATE_STRUCT 1

typedef struct {
    uint8_t  type;
    char     name[255];
    uint32_t start_offset;
    char     type_name[64];
    uint32_t type_size;;
    char     color_fg[4];
    char     color_bg[4];
    char     comment[255];
} template_entry_t;

typedef struct {
    uint8_t           type;
    char              name[255];
    int               entry_amount;
    template_entry_t* entries;
    char              color_fg[4];
    char              color_bg[4];
    char              comment[255];
} template_struct_t;

typedef struct {
    int               entry_amount;
    template_entry_t* entries;
} template_root_t;

//TODO: make entry allocation functions

int mycroft_parse_template_str(template_root_t* root, char* template_str);
int mycroft_parse_template_file(template_root_t* root, char* template_file);
