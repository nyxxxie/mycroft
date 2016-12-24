#include "template/type.h"

mc_error_t from_string(mc_type_t* type, const char* value)
{
    return MC_OK;
}

char* to_string(mc_type_t* type)
{
    return NULL;
}

MC_TYPE_REGISTER_BUILTIN("char", to_string, from_string);
