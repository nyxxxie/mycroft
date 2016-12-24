#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>
#include <mycroft/error.h>

typedef struct mc_type_t mc_type_t;
typedef struct mc_type_definition_t mc_type_definition_t;

typedef char* (*to_string_fn_t)(mc_type_t*);
typedef mc_error_t (*from_string_fn_t)(mc_type_t*, const char*);

struct mc_type_definition_t {
    const char* name;
    to_string_fn_t to_string_fn;
    from_string_fn_t from_string_fn;
};

#define MC_TYPE_REGISTER_BUILTIN(__name, __to_string_fn, __from_string_fn) \
    static mc_type_definition_t default_type                          \
    __attribute((used, section("default_types"))) = {      \
        .name           = __name,                          \
        .to_string_fn   = __to_string_fn,                  \
        .from_string_fn = __from_string_fn                 \
    }

// TODO: create second macro for use for runtime additions

#ifdef __cplusplus
}
#endif

#endif // TYPE_H
