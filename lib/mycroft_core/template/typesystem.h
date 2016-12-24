#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "template/type.h"

typedef struct {
    mc_type_definition_t* typedefs;
    size_t typedef_amount;
} mc_typesystem_t;

mc_typesystem_t* mc_typesystem_create();
void mc_typesystem_free(mc_typesystem_t* ts);

mc_error_t mc_typesystem_add_type(mc_type_definition_t* td);
mc_error_t mc_typesystem_create_type(to_string_fn_t to_string, from_string_fn_t from_string);

#ifdef __cplusplus
}
#endif

#endif // TYPESYSTEM_H
