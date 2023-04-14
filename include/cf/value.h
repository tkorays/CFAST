#ifndef __CF_VALUE_H__
#define __CF_VALUE_H__

#include "cf/types.h"

CF_DECLS_BEGIN

typedef enum {
    CF_VALUE_CHAR   = 1,
    CF_VALUE_INT8   = 1,
    CF_VALUE_UCHAR  = 2,
    CF_VALUE_UINT8  = 2,
    CF_VALUE_INT16,
    CF_VALUE_UINT16,
    CF_VALUE_INT32,
    CF_VALUE_UINT32,
    CF_VALUE_INT64,
    CF_VALUE_UINT64,
    CF_VALUE_FLOAT,
    CF_VALUE_DOUBLE,
    CF_VALUE_STRING,
    CF_VALUE_CUSTOM,
} cf_value_type_t;

typedef struct cf_value cf_value_t;


typedef struct cf_value {
    cf_uint32_t     type: 8;
    cf_uint32_t     len: 24;
    union {
        cf_char_t   c;
        cf_int8_t   i8;
        cf_uchar_t  uc;
        cf_uint8_t  u8;
        cf_int16_t  i16;
        cf_uint16_t u16;
        cf_int32_t  i32;
        cf_uint32_t u32;
        cf_int64_t  i64;
        cf_uint64_t u64;
        cf_char_t*  str;
        cf_void_t*  ptr;
    } data;
} cf_value_t;


CF_FORCE_INLINE cf_void_t cf_value_init_char(cf_value_t* v, cf_char_t v) {
    v->type = CF_VALUE_CHAR;
    v->len = sizeof(cf_char_t);
    v->data.c = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_uchar(cf_value_t* v, cf_uchar_t v) {
    v->type = CF_VALUE_UCHAR;
    v->len = sizeof(cf_uchar_t);
    v->data.uc = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_i8(cf_value_t* v, cf_int8_t v) {
    v->type = CF_VALUE_INT8;
    v->len = sizeof(cf_int8_t);
    v->data.i8 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_u8(cf_value_t* v, cf_uint8_t v) {
    v->type = CF_VALUE_UINT8;
    v->len = sizeof(cf_uint8_t);
    v->data.u8 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_i16(cf_value_t* v, cf_int16_t v) {
    v->type = CF_VALUE_INT16;
    v->len = sizeof(cf_int16_t);
    v->data.i16 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_u16(cf_value_t* v, cf_uint16_t v) {
    v->type = CF_VALUE_UINT16;
    v->len = sizeof(cf_uint16_t);
    v->data.u16 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_i32(cf_value_t* v, cf_int32_t v) {
    v->type = CF_VALUE_INT32;
    v->len = sizeof(cf_int32_t);
    v->data.i32 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_u32(cf_value_t* v, cf_uint32_t v) {
    v->type = CF_VALUE_UINT32;
    v->len = sizeof(cf_uint32_t);
    v->data.u32 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_i64(cf_value_t* v, cf_int64_t v) {
    v->type = CF_VALUE_INT64;
    v->len = sizeof(cf_int64_t);
    v->data.i64 = v;
}

CF_FORCE_INLINE cf_void_t cf_value_init_u64(cf_value_t* v, cf_uint64_t v) {
    v->type = CF_VALUE_UINT64;
    v->len = sizeof(cf_uint64_t);
    v->data.u64 = v;
}


CF_FORCE_INLINE cf_void_t cf_value_init_str(cf_value_t* v, const cf_char_t* v) {
    v->type = CF_VALUE_STRING;
    v->len = strlen(v) + 1;
    v->data.str = malloc(v->len);
    memcpy_s(v->data.str, v->len, v, v->len);
}

CF_FORCE_INLINE cf_void_t cf_value_init_data(cf_value_t* v, cf_void_t* v, cf_size_t len) {
    
}

cf_void_t cf_value_deinit(cf_value_t* v);

CF_DECLS_END

#endif /* __CF_VALUE_H__ */
