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

/**
 * String.
 */
typedef struct {
    cf_char_t*  data;   /** string data without \0 */
    cf_size_t   length; /** string length */
} cf_string_t;

/**
 * Const string, data is protected.
 */
typedef struct {
    const cf_char_t*    data;   /** string data without \0 */
    cf_size_t           length; /** string length */
} cf_string_const_t;

/**
 * Create a string.
 * @param size      Size of string memory.
 * @return          Instance of cf_string_t.
 */
cf_string_t*    cf_str_new(cf_size_t size);

/**
 * Free a string.
 * @param s         string.
 * @return          None.
 */
cf_void_t       cf_str_free(cf_string_t* s);

/**
 * Free a const string.
 * @param s         string.
 * @return          None.
 */
cf_void_t       cf_str_cont_free(cf_string_const_t* s);

/**
 * Create a string from char*.
 * @param pc        char* string.
 * @return          cf_string_t.
 */
cf_string_t*    cf_str_from_pchar(cf_char_t* pc);

/**
 * Get length of a string.
 * @param s         String.
 * @return          Length.
 */
cf_size_t       cf_str_len(cf_string_t* s);

/**
 * Get length of a string.
 * @param s         String.
 * @param ppc       char**.
 * @return          None.
 */
cf_void_t       cf_str_to_pchar(cf_string_t* s, cf_char_t** ppc);

#endif /* __CF_STRING_H__ */
