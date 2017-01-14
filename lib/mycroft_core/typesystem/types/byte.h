#include "typesystem/typedef.h"

mc_error_t byte_to_string(mc_typedef_t* type, uint8_t* bytes, size_t byte_amount, char** strbuf, size_t* strbuf_size);
mc_error_t byte_from_string(mc_typedef_t* type, const char* string, uint8_t** bytes, size_t* byte_amount);
