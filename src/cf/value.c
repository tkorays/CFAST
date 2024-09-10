#include "cf/value.h"
#include "cf/memory.h"
#include "cf/str.h"
#include "cf/types.h"
#include <string.h>

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

cf_void_t* cf_value_set_data(cf_value_t* value, cf_void_t* v, cf_size_t len) {
    value->type = CF_VALUE_CUSTOM;
    value->len = len;
    value->data.ptr = cf_malloc(len);
    if (v) {
        cf_memcpy_s(value->data.ptr, value->len, v, len);
    }
    return value->data.ptr;
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

cf_int_t cf_value_cmp(const cf_value_t* a, const cf_value_t* b) {
    cf_assert(a->type == b->type);
    switch (a->type) {
    case CF_VALUE_CHAR:
        return a->data.c == b->data.c ? 0 : (a->data.c > b->data.c ? 1 : -1);
    case CF_VALUE_UCHAR:
        return a->data.u8 == b->data.u8 ? 0 : (a->data.u8 > b->data.u8 ? 1 : -1);
    case CF_VALUE_INT16:
        return a->data.i16 == b->data.i16 ? 0 : (a->data.i16 > b->data.i16 ? 1 : -1);
    case CF_VALUE_UINT16:
        return a->data.u16 == b->data.u16 ? 0 : (a->data.u16 > b->data.u16 ? 1 : -1);
    case CF_VALUE_INT32:
        return a->data.i32 == b->data.i32 ? 0 : (a->data.i32 > b->data.i32 ? 1 : -1);
    case CF_VALUE_UINT32:
        return a->data.u32 == b->data.u32 ? 0 : (a->data.u32 > b->data.u32 ? 1 : -1);
    case CF_VALUE_INT64:
        return a->data.i64 == b->data.i64 ? 0 : (a->data.i64 > b->data.i64 ? 1 : -1);
    case CF_VALUE_UINT64:
        return a->data.u64 == b->data.u64 ? 0 : (a->data.u64 > b->data.u64 ? 1 : -1);
    case CF_VALUE_FLOAT:
        return a->data.f32 == b->data.f32 ? 0 : (a->data.f32 > b->data.f32 ? 1 : -1);
    case CF_VALUE_DOUBLE:
        return a->data.f64 == b->data.f64 ? 0 : (a->data.f64 > b->data.f64 ? 1 : -1);
    case CF_VALUE_STRING:
        return strcmp(
                cf_value_string(CF_TYPE_CAST(cf_value_t*, a)),
                cf_value_string(CF_TYPE_CAST(cf_value_t*, b))
            );
    case CF_VALUE_CUSTOM: {
        int ret = memcmp(a->data.ptr, b->data.ptr, CF_MIN2(a->len, b->len));
        if (ret == 0) {
            return a->len == b->len ? 0 : (a->len < b->len ? -1 : 1);
        }
        return ret;
    }
    default:
        return 0;
    }
}
