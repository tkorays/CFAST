#ifndef __CF_VALUE_H__
#define __CF_VALUE_H__

#include "cf/types.h"

CF_DECLS_BEGIN

typedef enum {
    CF_VALUE_NONE   = 0,
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


/**
 * Definition for a value, which can hold various types of data.
 * The type of data is indicated by the 'type' field.
 * The 'len' field indicates the length of the data in bytes.
 * The actual data is stored in the 'data' union.
 */
typedef struct cf_value {
    cf_uint32_t     type: 8;    /**< 8 bits for the type field */
    cf_uint32_t     len: 24;    /**< 24 bits for the length field */
    union {
        cf_char_t   c;          /**< character data */
        cf_int8_t   i8;         /**< 8-bit integer data */
        cf_uchar_t  uc;         /**< unsigned character data */
        cf_uint8_t  u8;         /**< unsigned 8-bit integer data */
        cf_int16_t  i16;        /**< 16-bit integer data */
        cf_uint16_t u16;        /**< unsigned 16-bit integer data */
        cf_int32_t  i32;        /**< 32-bit integer data */
        cf_uint32_t u32;        /**< unsigned 32-bit integer data */
        cf_int64_t  i64;        /**< 64-bit integer data */
        cf_uint64_t u64;        /**< unsigned 64-bit integer data */
        float       f32;        /**< 32-bit floating point data */
        double      f64;        /**< 64-bit floating point data */
        cf_char_t   arr[8];     /**< char array for small string */
        cf_char_t*  str;        /**< string data */
        cf_void_t*  ptr;        /**< pointer data */
    } data;
} cf_value_t;



/**
 * @brief initialize a value
 * @param value the value to be initialized
 * @return cf_void_t
 */
cf_void_t cf_value_init(cf_value_t* value);

/**
 * @brief Deinitializes a value
 * 
 * @param value the value to be deinitialized
 * @return cf_void_t
 */
cf_void_t cf_value_deinit(cf_value_t* value);

/**
 * @brief Set the value of a cf_value_t struct to a single character
 * 
 * @param value the value to be set
 * @param v the character to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_char(cf_value_t* value, cf_char_t v) {
    value->type = CF_VALUE_CHAR;
    value->len = sizeof(cf_char_t);
    value->data.c = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an unsigned character
 * 
 * @param value the value to be set
 * @param v the unsigned character to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_uchar(cf_value_t* value, cf_uchar_t v) {
    value->type = CF_VALUE_UCHAR;
    value->len = sizeof(cf_uchar_t);
    value->data.uc = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an 8-bit integer
 * 
 * @param value the value to be set
 * @param v the 8-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_i8(cf_value_t* value, cf_int8_t v) {
    value->type = CF_VALUE_INT8;
    value->len = sizeof(cf_int8_t);
    value->data.i8 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an unsigned 8-bit integer
 * 
 * @param value the value to be set
 * @param v the unsigned 8-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_u8(cf_value_t* value, cf_uint8_t v) {
    value->type = CF_VALUE_UINT8;
    value->len = sizeof(cf_uint8_t);
    value->data.u8 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to a 16-bit integer
 * 
 * @param value the value to be set
 * @param v the 16-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_i16(cf_value_t* value, cf_int16_t v) {
    value->type = CF_VALUE_INT16;
    value->len = sizeof(cf_int16_t);
    value->data.i16 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an unsigned 16-bit integer
 * 
 * @param value the value to be set
 * @param v the unsigned 16-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_u16(cf_value_t* value, cf_uint16_t v) {
    value->type = CF_VALUE_UINT16;
    value->len = sizeof(cf_uint16_t);
    value->data.u16 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to a 32-bit integer
 * 
 * @param value the value to be set
 * @param v the 32-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_i32(cf_value_t* value, cf_int32_t v) {
    value->type = CF_VALUE_INT32;
    value->len = sizeof(cf_int32_t);
    value->data.i32 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an unsigned 32-bit integer
 * 
 * @param value the value to be set
 * @param v the unsigned 32-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_u32(cf_value_t* value, cf_uint32_t v) {
    value->type = CF_VALUE_UINT32;
    value->len = sizeof(cf_uint32_t);
    value->data.u32 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to a 64-bit integer
 * 
 * @param value the value to be set
 * @param v the 64-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_i64(cf_value_t* value, cf_int64_t v) {
    value->type = CF_VALUE_INT64;
    value->len = sizeof(cf_int64_t);
    value->data.i64 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to an unsigned 64-bit integer
 * 
 * @param value the value to be set
 * @param v the unsigned 64-bit integer to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_u64(cf_value_t* value, cf_uint64_t v) {
    value->type = CF_VALUE_UINT64;
    value->len = sizeof(cf_uint64_t);
    value->data.u64 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to a 32-bit floating point number
 * 
 * @param value the value to be set
 * @param v the 32-bit floating point number to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_float(cf_value_t* value, float v) {
    value->type = CF_VALUE_FLOAT;
    value->len = sizeof(float);
    value->data.f32 = v;
}

/**
 * @brief Set the value of a cf_value_t struct to a 64-bit floating point number
 * 
 * @param value the value to be set
 * @param v the 64-bit floating point number to set the value to
 */
CF_FORCE_INLINE cf_void_t cf_value_set_double(cf_value_t* value, double v) {
    value->type = CF_VALUE_DOUBLE;
    value->len = sizeof(double);
    value->data.f64 = v;
}


/**
 * @brief Set the value of a cf_value_t struct to a string
 * 
 * @param value the value to be set
 * @param v the string to set the value to
 */
cf_void_t cf_value_set_str(cf_value_t* value, const cf_char_t* v);

/**
 * @brief Set the value of a cf_value_t struct to a block of data
 * 
 * @param value the value to be set
 * @param v the block of data to set the value to
 * @param len the length of the block of data
 */
cf_void_t cf_value_set_data(cf_value_t* value, cf_void_t* v, cf_size_t len);

/**
 * @brief Get the value of a cf_value_t struct as a character
 * 
 * @param value the value to get
 * @return the character value
 */
CF_FORCE_INLINE cf_char_t cf_value_char(cf_value_t* value) { return value->data.c; }

/**
 * @brief Get the value of a cf_value_t struct as an 8-bit integer
 * 
 * @param value the value to get
 * @return the 8-bit integer value
 */
CF_FORCE_INLINE cf_int8_t cf_value_int8(cf_value_t* value) { return value->data.i8; }

/**
 * @brief Get the value of a cf_value_t struct as an unsigned character
 * 
 * @param value the value to get
 * @return the unsigned character value
 */
CF_FORCE_INLINE cf_uchar_t cf_value_uchar(cf_value_t* value) { return value->data.uc; }

/**
 * @brief Get the value of a cf_value_t struct as an unsigned 8-bit integer
 * 
 * @param value the value to get
 * @return the unsigned 8-bit integer value
 */
CF_FORCE_INLINE cf_uint8_t cf_value_uint8(cf_value_t* value) { return value->data.u8; }

/**
 * @brief Get the value of a cf_value_t struct as a 16-bit integer
 * 
 * @param value the value to get
 * @return the 16-bit integer value
 */
CF_FORCE_INLINE cf_int16_t cf_value_int16(cf_value_t* value) { return value->data.i16; }

/**
 * @brief Get the value of a cf_value_t struct as an unsigned 16-bit integer
 * 
 * @param value the value to get
 * @return the unsigned 16-bit integer value
 */
CF_FORCE_INLINE cf_uint16_t cf_value_uint16(cf_value_t* value) { return value->data.u16; }

/**
 * @brief Get the value of a cf_value_t struct as a 32-bit integer
 * 
 * @param value the value to get
 * @return the 32-bit integer value
 */
CF_FORCE_INLINE cf_int32_t cf_value_int32(cf_value_t* value) { return value->data.i32; }

/**
 * @brief Get the value of a cf_value_t struct as an unsigned 32-bit integer
 * 
 * @param value the value to get
 * @return the unsigned 32-bit integer value
 */
CF_FORCE_INLINE cf_uint32_t cf_value_uint32(cf_value_t* value) { return value->data.u32; }

/**
 * @brief Get the value of a cf_value_t struct as a 64-bit integer
 * 
 * @param value the value to get
 * @return the 64-bit integer value
 */
CF_FORCE_INLINE cf_int64_t cf_value_int64(cf_value_t* value) { return value->data.i64; }

/**
 * @brief Get the value of a cf_value_t struct as an unsigned 64-bit integer
 * 
 * @param value the value to get
 * @return the unsigned 64-bit integer value
 */
CF_FORCE_INLINE cf_uint64_t cf_value_uint64(cf_value_t* value) { return value->data.u64; }

/**
 * @brief Get the value of a cf_value_t struct as a float
 * 
 * @param value the value to get
 * @return the float value
 */
CF_FORCE_INLINE float cf_value_float(cf_value_t* value) { return value->data.f32; }

/**
 * @brief Get the value of a cf_value_t struct as a double
 * 
 * @param value the value to get
 * @return the double value
 */
CF_FORCE_INLINE double cf_value_double(cf_value_t* value) { return value->data.f64; }

/**
 * @brief Get the value of a cf_value_t struct as a string
 * 
 * @param value the value to get
 * @return the string value
 */
CF_FORCE_INLINE cf_char_t* cf_value_string(cf_value_t* value) {
    if (value->len <= 8) return &value->data.arr[0];
    return value->data.str;
}

/**
 * @brief Get the value of a cf_value_t struct as a custom data type
 * 
 * @param value the value to get
 * @return the custom data type value
 */
CF_FORCE_INLINE cf_void_t* cf_value_custom(cf_value_t* value) { return value->data.ptr; }


CF_DECLS_END

#endif /* __CF_VALUE_H__ */
