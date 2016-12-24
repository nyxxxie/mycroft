#include "template/typesystem.h"
#include "template/type.h"

/**
 * Function that copies all of the types registered with the MC_TYPE_REGISTER
 * macro to the given type system.  Don't call this function outside of
 * mc_typesystem_create(), as that is the only place this operation is
 * necessary
 * @internal
 */
mc_error_t add_default_types(mc_typesystem_t* ts)
{

    //http://stackoverflow.com/questions/3633896/append-items-to-an-array-with-a-macro-in-c
}

mc_typesystem_t* mc_typesystem_create()
{
    mc_error_t rc = MC_ERR;
    mc_typesystem_t* ret = NULL;

    //allocate typesystem

    // initialize struct

    /* Add default types to this typesystem */
    rc = add_default_types(ret);
    if (rc != MC_OK) {
        return NULL;
    }

    return ret;
}

void mc_typesystem_free(mc_typesystem_t* ts)
{

}
