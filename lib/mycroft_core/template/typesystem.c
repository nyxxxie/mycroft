#include <stdio.h>
#include <stdlib.h>
#include "template/typesystem.h"
#include "template/typedef.h"

/**
 * Function that copies all of the types registered with the MC_TYPE_REGISTER
 * macro to the given type system.  Don't call this function outside of
 * mc_typesystem_create(), as that is the only place this operation is
 * necessary
 * @internal
 */
mc_error_t add_default_types(mc_typesystem_t* ts)
{
    mc_typedef_t* type = NULL;

    return MC_OK;
}

mc_typesystem_t* mc_typesystem_create()
{
    mc_error_t rc = MC_ERR;
    mc_typesystem_t* ret = NULL;

    /* */
    ret = (mc_typesystem_t*)malloc(sizeof(*ret));
    if (ret == NULL) {
        MC_ERROR("Failed to allocate typesystem.\n");
        return NULL;
    }

    /* */
    ret->typedefs = NULL;
    ret->typedef_amount = 0;

    /* Add default types to this typesystem */
    rc = add_default_types(ret);
    if (rc != MC_OK) {
        MC_ERROR("Failed to add default types.\n");
        return NULL;
    }

    return ret;
}

void mc_typesystem_free(mc_typesystem_t* ts)
{
    // TODO: free type array

    free(ts);
}

mc_error_t mc_typesystem_register_type(mc_typesystem_t* ts, mc_typedef_t* type_new)
{
    mc_error_t rc = MC_ERR;
    size_t cur_index = 0;
    size_t i = 0;

    /* */
    if (type_new == NULL) {
        MC_ERROR("Input type is null.\n");
        return rc;
    }

    /* Ensure we haven't already added the file */
    if (ts->typedefs != NULL) {
        for (i=0; i < ts->typedef_amount; i++) {
            mc_typedef_t* type = ts->typedefs[i];
            if (type_new == type || strcmp(type->name, type_new->name) == 0) {
                MC_ERROR("Type already exists in typesystem.\n");
                return rc;
            }
        }
    }

    /* */
    cur_index = ts->typedef_amount;
    ts->typedef_amount++;

    /* */
    ts->typedefs = (mc_typedef_t**)realloc(ts->typedefs, sizeof(*ts->typedefs) * ts->typedef_amount);
    if (ts->typedefs == NULL) {
        MC_ERROR("Failed to (re)alloc typedef array.\n");
        return rc;
    }

    /* Add type to type array */
    ts->typedefs[cur_index] = type_new;

    return MC_OK;
}

mc_error_t mc_typesystem_create_type(mc_typesystem_t* ts, const char* name, uint64_t size, to_string_fn_t to_string, from_string_fn_t from_string)
{
    mc_error_t rc = MC_ERR;
    mc_typedef_t* type = NULL;

    /* */
    type = mc_typedef_create();
    if (type == NULL) {
        MC_ERROR("Failed to allocate space for types.\n");
        return MC_ERR;
    }

    /* */
    rc = mc_typedef_set_name(type, name);
    if (rc != MC_OK) {
        MC_ERROR("Failed to set type name.\n");
        return MC_ERR;
    }

    type->size = size;
    type->to_string_fn = to_string;
    type->from_string_fn = from_string;

    return mc_typesystem_register_type(ts, type);
}

mc_typedef_t* mc_typesystem_get_type(mc_typesystem_t* ts, const char* type_name)
{
    size_t i = 0;

    /* Ensure we haven't already added the file */
    for (i=0; i < ts->typedef_amount; i++) {
        mc_typedef_t* type = ts->typedefs[i];
        if (strcmp(type->name, type_name) == 0) {
            return type;
        }
    }

    /* Couldn't find the given type */
    return NULL;
}
