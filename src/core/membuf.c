#include <string.h>
#include "membuf.h"

int membuf_init(membuf_t* mb, membuf_type type) {
    mb->size = 0;
    mb->data = NULL;
    mb->type = type;
}

int membuf_copybytes(membuf_t* mb, void* data, int size) {

    /* Alloc memory */
    mb->size = size;
    mb->data = malloc(size);
    if (mb->data == NULL) {
        return -1;
    }

    /* Copy memory */
    memcpy(mb->data, data, size);

    return 0;
}

int membuf_get_size(membuf_t* mb) {
    return mb->size;
}

void* membuf_get_data(membuf_t* mb) {
    return mb->data;
}

int membuf_close(membuf_t* mb) {
    if (mb->data != NULL) {
        free(mb->data);
    }
}
