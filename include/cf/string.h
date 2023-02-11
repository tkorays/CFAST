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
#include <cf/memory.h>

/**
 * @defgroup CF_ADV_STRING advance string
 * advance string implementation
 * @{
 */

CF_DECLS_BEGIN

typedef struct {
    cf_uint32_t         ref : 1;    /** this is a reference object */
    cf_uint32_t         len : 31;   /** string length */
    cf_char_t*          ptr;        /** string data with \0 */
} cf_string_ref_t;

/**
 * String.
 */
typedef struct {
    cf_uint32_t         ref : 1;    /** this is a reference object */
    cf_uint32_t         len : 31;   /** string length */
    cf_char_t*          ptr;        /** string data with \0 */
    cf_allocator_t*     allocator;  /** null allocator means unmanaged data */
} cf_string_t;


/**
 * @brief initialize a string object with length.
 * 
 * @param self this pointer
 * @param len  string length
 * @param str  initial string
 * @return cf_bool_t success or failed.
 */
cf_bool_t cf_string_init(cf_string_t* self, cf_size_t len, const cf_char_t* str);

/**
 * @brief initialize a string object with lengh and a valid allocator
 * 
 * @param self  this pointer
 * @param len   length of this string
 * @param str  initial string
 * @param alloc allocator
 * @return cf_bool_t 
 */
cf_bool_t cf_string_init_with_allocator(cf_string_t* self, cf_size_t len, const cf_char_t* str, cf_allocator_t* alloc);

/**
 * @brief destroy a string object.
 * 
 * @param self this pointer
 */
void cf_string_deinit(cf_string_t* self);

/**
 * @brief compare two stings
 * 
 * @param self  this pointer
 * @param s     the second string
 * @param s_len the second string length
 * @return int  compare result, like strcmp
 */
int cf_string_cmp_raw(cf_string_t* self, const char* s, cf_size_t s_len);


/**
 * @brief Reset a string object with other string.
 * 
 * @param self  this pointer
 * @param ptr   string pointer
 * @param len   string length
 * @param alloc allocator, a null allocator means this is a reference string.
 */
void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len, cf_allocator_t* alloc);

/**
 * @brief string length
 * 
 */
#define cf_string_len(self) ((self)->len)

/**
 * @brief string pointer
 * 
 */
#define cf_string_ptr(self) ((self)->ptr)

/**
 * @brief clear string object
 * 
 */
#define cf_string_clear(self) do { (self)->ptr = CF_NULL_PTR; (self)->len = 0; (self)->allocator = CF_NULL_PTR; } while(0)

/**
 * @brief init a string with reference
 * 
 */
#define cf_string_init_with_ref(self, s, l) do { (self)->ptr = (s); (self)->len = (l); (self)->allocator = CF_NULL_PTR; } while(0)

/**
 * @brief compare two cf_string_t object
 * 
 */
#define cf_string_cmp(self, other) cf_string_cmp_raw((self), (other)->ptr, (other)->len)

CF_DECLS_END

/** @} */

#endif /* __CF_STRING_H__ */
