#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "template/typedef.h"

typedef struct {
    mc_typedef_t** typedefs;
    size_t typedef_amount;
} mc_typesystem_t;

mc_typesystem_t* mc_typesystem_create();
void mc_typesystem_free(mc_typesystem_t* ts);

mc_error_t mc_typesystem_register_type(mc_typesystem_t* td, mc_typedef_t* type_new);
mc_error_t mc_typesystem_create_type(mc_typesystem_t* ts, const char* name, uint64_t size, to_string_fn_t to_string, from_string_fn_t from_string);

mc_typedef_t* mc_typesystem_get_type(mc_typesystem_t* ts, const char* type_name);

#ifdef __cplusplus
}
#endif

#endif // TYPESYSTEM_H
