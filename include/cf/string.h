/****************************************************
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
#include <cf/str.h>

/**
 * String.
 */
typedef struct {
    cf_char_t*  ptr;   /** string data without \0 */
    cf_size_t   len; /** string length */
} cf_string_t;

inline cf_string_t cf_string(cf_char_t* s) {
    cf_string_t str;
    str.ptr = s;
    str.len = s ? cf_strlen(s) : 0;
    return str;
}

inline cf_string_t* cf_string_from_c(cf_string_t* str, const cf_char_t* s) {
    if(!str) return CF_NULL_PTR;
    str->ptr = (cf_char_t*)s;
    str->len = s ? cf_strlen(s) : 0;
    return str;
}

inline cf_string_t* cf_string_set(cf_string_t* str, const cf_char_t* s, cf_size_t len) {
    if(!str) return CF_NULL_PTR;
    str->ptr = (cf_char_t*)s;
    str->len = len;
    return str;
}

inline cf_size_t cf_string_len(cf_string_t* str) {
    return str ? str->len : 0;
}

#endif /* __CF_STRING_H__ */
