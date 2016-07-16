#ifndef MEMBUF_H
#define MEMBUF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEMBUF_HEAP,
    MEMBUF_STACK    // This doesn't work, DON'T USE IT.
} membuf_type;

typedef struct {
    int   size;
    void* data;
    membuf_type type;
} membuf_t;

int membuf_init(membuf_t* mb, membuf_type type);
int membuf_copybytes(membuf_t* mb, void* data, int size);
int membuf_get_size(membuf_t* mb);
void* membuf_get_data(membuf_t* mb);
int membuf_close(membuf_t* mb);

#ifdef __cplusplus
}
#endif

#endif // MEMBUF_INT_H
