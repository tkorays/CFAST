/***************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    string.h
 * @brief   String.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-9
 ****************************************************/
#ifndef __CF_STRING_H__
#define __CF_STRING_H__

#include <cf/types.h>

/**
 * @defgroup CF_ADV_STRING advance string
 * advance string implementation
 * @{
 */

CF_DECLS_BEGIN

/**
 * String.
 */
typedef struct {
    cf_uint32_t         ref : 1;    /** this is a reference object */
    cf_uint32_t         len : 31;   /** string length */
    cf_char_t*          ptr;        /** string data with \0 */
} cf_string_t;


/**
 * @brief initialize a string object with length.
 * 
 * @param self this pointer
 * @param len  string length
 * @param str  initial string
 * @return cf_bool_t success or failed.
 */
cf_bool_t cf_string_init(cf_string_t* self, const cf_char_t* str, cf_size_t len);


/**
 * @brief destroy a string object.
 * 
 * @param self this pointer
 */
void cf_string_deinit(cf_string_t* self);


/**
 * @brief clear string object
 *
 * this function is unsafe!!! you should be care about releasing memory.
 * 
 */
CF_FORCE_INLINE void cf_string_clear(cf_string_t* str) {
    str->ref = CF_FALSE;
    str->ptr = CF_NULL_PTR;
    str->len = 0;
}

/**
 * @brief take a c string and own this memory
 * 
 * @param   str     string pointer
 * @param   len     length used in this string
 *
 * @return  a string instance.
 */
CF_FORCE_INLINE cf_string_t cf_string_take_c(cf_char_t* str, cf_size_t len) {
    cf_string_t s;
    s.ref   = CF_FALSE;
    s.len   = len;
    s.ptr   = str;
    return s;
}

/**
 * @brief take and own memory from other string instance.
 *
 * @param   other   other string 
 *
 * @return a new string instance
 */
CF_FORCE_INLINE cf_string_t cf_string_take(cf_string_t* other) {
    cf_string_t s;
    s.ref   = CF_FALSE;
    s.len   = other->len;
    s.ptr   = other->ptr;

    other->ref  = CF_FALSE;
    other->ptr  = CF_NULL_PTR;
    other->len  = 0;

    return s;
}

/**
 * @brief reference a c string and share the memory with other object.
 * 
 * @param   str     string pointer
 * @param   len     length used in this string
 *
 * @return  a string instance.
 */
CF_FORCE_INLINE cf_string_t cf_string_ref_c(cf_char_t* str, cf_size_t len) {
    cf_string_t s;
    s.ref   = CF_TRUE;
    s.len   = len;
    s.ptr   = str;
    return s;
}

/**
 * @brief reference a string and share the memory with other object.
 * 
 * @param   str     string pointer
 * @param   len     length used in this string
 *
 * @return  a string instance.
 */
CF_FORCE_INLINE cf_string_t cf_string_ref(cf_string_t* other) {
    cf_string_t s;
    s.ref   = CF_TRUE;
    s.len   = other->len;
    s.ptr   = other->ptr;
    return s;
}

/**
 * @brief duplication: copy from other cf_string_t.
 * 
 * @param   other   other string 
 *
 * @return  a string instance.
 */
CF_FORCE_INLINE cf_string_t cf_string_dup_c(const cf_char_t* s, cf_size_t len) {
    cf_string_t str;
    cf_string_clear(&str);
    cf_string_init(&str, s, len);
    return str;
}

/**
 * @brief duplication: copy from other cf_string_t.
 * 
 * @param   other   other string 
 *
 * @return  a string instance.
 */
CF_FORCE_INLINE cf_string_t cf_string_dup(const cf_string_t* other) {
    cf_string_t str;
    cf_string_clear(&str);
    cf_string_init(&str, other->ptr, other->len);
    return str;
}

/**
 * @brief compare two stings
 * 
 * @param self  this pointer
 * @param s     the second string
 * @param s_len the second string length
 * @return int  compare result, like strcmp
 */
int cf_string_cmp_raw(const cf_string_t* self, const char* s, cf_size_t s_len);


/**
 * @brief compare two stings
 * 
 * @param self  this pointer
 * @param other othter string
 * @return int  compare result, like strcmp
 */
CF_FORCE_INLINE int cf_string_cmp(const cf_string_t* self, const cf_string_t* other) {
    return cf_string_cmp_raw(self, other->ptr, other->len);
}


/**
 * @brief Reset a string object with other string.
 * 
 * @param self  this pointer
 * @param ptr   string pointer
 * @param len   string length
 */
void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len);

/**
 * @brief string length
 * 
 */
CF_FORCE_INLINE cf_size_t cf_string_len(const cf_string_t* str) {
    return str->len;
}

/**
 * @brief string pointer
 * 
 */
CF_FORCE_INLINE const cf_char_t* cf_string_ptr(const cf_string_t* str) {
    return str->ptr;
}

CF_DECLS_END

/** @} */

#endif /* __CF_STRING_H__ */
