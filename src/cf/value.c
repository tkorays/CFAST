#include "cf/value.h"
#include "cf/memory.h"
#include "cf/str.h"

cf_void_t cf_value_set_str(cf_value_t* value, const cf_char_t* v) {
    value->type = CF_VALUE_STRING;
    value->len = cf_strlen(v) + 1;
    if (value->len <= 8) {
        cf_memcpy_s(&value->data.arr[0], sizeof(value->data.arr), v, value->len);
    } else {
        value->data.ptr = cf_malloc(value->len);
        cf_memcpy_s(value->data.ptr, value->len, v, value->len);
    }
}

cf_void_t cf_value_set_data(cf_value_t* value, cf_void_t* v, cf_size_t len) {
    value->type = CF_VALUE_CUSTOM;
    value->len = len;
    value->data.ptr = cf_malloc(len);
    cf_memcpy_s(value->data.ptr, value->len, v, len);
}


cf_void_t cf_value_init(cf_value_t* value) {
    value->type = CF_VALUE_NONE;
    value->len = 0;
    value->data.ptr = CF_NULL_PTR;
}

cf_void_t cf_value_deinit(cf_value_t* value) {
    if (value->type != CF_VALUE_STRING && value->type != CF_VALUE_CUSTOM) {
        goto finish;
    }
    if (value->type == CF_VALUE_STRING && value->len <= 8) {
        goto finish;
    }
    if (value->data.ptr && value->type != CF_VALUE_POINTER) {
        cf_free(value->data.ptr);
    }
finish:
    cf_value_init(value);
}
