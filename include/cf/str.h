/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    str.h
 * @brief   String functions for standard library.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-10
 ****************************************************/
#ifndef __CF_STR_H__
#define __CF_STR_H__

#include <cf/types.h>

/**
 * Calculate length of a string.
 * @param s         String.
 * @return          Length of a string.
 */
cf_size_t cf_strlen(const cf_char_t* s);

/**
 * Compare two strings.
 * @param s1        String.
 * @param s2        String.
 * @return          Compararision result.
 */
cf_int32_t cf_strcmp(const cf_char_t* s1, const cf_char_t* s2);

/**
 * Compare two strings without case consideration.
 * @param s1        String.
 * @param s2        String.
 * @return          Compararision result.
 */
cf_int32_t cf_strcasecmp(const cf_char_t* s1, const cf_char_t* s2);

/**
 * Link two strings.
 * @param dst       Destination string.
 * @param dstsize   Size of destination string.
 * @param src       Source string.
 * @return          Pointer to destination if link successfully.
 */
cf_char_t* cf_strcat_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src);

/**
 * Copy string to another one.
 * @param dst       Destination string.
 * @param dstsize   Size of destination string.
 * @param src       Source string.
 * @return          Pointer to destination if link successfully.
 */
cf_char_t* cf_strcpy_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src);

/**
 * Substring of a string.
 * @param s1        Source string to search into.
 * @param s2        Destination string.
 * @return          Pointer to start if search successfully.
 */
cf_char_t* cf_strstr(cf_char_t* s1, cf_char_t* s2);

/**
 * Substring of a string reverse.(Not implemented!)
 * @param s1        Source string to search into.
 * @param s2        Destination string.
 * @return          Pointer to start if search successfully.
 */
cf_char_t* cf_strrstr(cf_char_t* s1, cf_char_t* s2);

/**
 * Index of a char.
 * @param s         Source string to search into.
 * @param c         Char to search.
 * @return          Pointer to start if search successfully.
 */
cf_char_t* cf_strchr(const cf_char_t* s, cf_char_t c);


#endif /* __CF_STR_H__ */
