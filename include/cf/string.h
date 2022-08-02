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

CF_DECLS_BEGIN

/**
 * String.
 */
typedef struct {
    cf_char_t*  ptr;    /** string data without \0 */
    cf_uint32_t ref:1;  /** string reference, not managed */
    cf_uint32_t len:31; /** string length */
} cf_string_t;


/**
 * @brief initialize a string object with length.
 * 
 * @param self this pointer
 * @param len  string length
 * @return cf_bool_t success or failed.
 */
cf_bool_t cf_string_init(cf_string_t* self, cf_size_t len);

/**
 * @brief destroy a string object.
 * 
 * @param self this pointer
 */
void cf_string_deinit(cf_string_t* self);

/**
 * @brief Reset a string object with other string.
 * 
 * @param self this pointer
 * @param ptr  string pointer
 * @param len  string length
 * @param ref  is reference or not.
 */
void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len, cf_bool_t ref);

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
#define cf_string_clear(self) do { (self)->ptr = CF_NULL_PTR; (self)->ref = CF_TRUE; (self)->len = 0; } while(0)


CF_DECLS_END

#endif /* __CF_STRING_H__ */
