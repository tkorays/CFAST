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

CF_DECLS_BEGIN

#define CF_IS_ASCII(c) ((c) < 128)
#define CF_IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define CF_IS_UPPER_ALPHA(c) ((c) >= 'A' && (c) <= 'Z')
#define CF_IS_LOWER_ALPHA(c) ((c) >= 'a' && (c) <= 'z')
#define CF_TO_LOWER_ALPHA(c) (CF_IS_UPPER_ALPHA(c) ? (c - 'A' + 'a') : (c))
#define CF_TO_UPPER_ALPHA(c) (CF_IS_LOWER_ALPHA(c) ? (c - 'a' + 'A') : (c))
#define CF_IS_ALPHA(c) (CF_IS_UPPER_ALPHA(c) || CF_IS_LOWER_ALPHA(c))
#define CF_IS_SPACE(c) ((cf_char_t)(c) == '\t' || (cf_char_t)(c) == '\n' || (cf_char_t)(c) == ' ')
#define CF_IS_BLANK(c) ((c) == '\t' || (c) == ' ')


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
cf_int32_t cf_stricmp(const cf_char_t* s1, const cf_char_t* s2);

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
 * Copy string to another one.
 * @param dst       Destination string.
 * @param dstsize   Size of destination string.
 * @param src       Source string.
 * @param srcmax    Size to be copy.
 * @return          Pointer to destination if link successfully.
 */
cf_char_t* cf_strncpy_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src, cf_size_t srcmax);

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
 * Index of a character.
 * @param s         Source string to search into.
 * @param c         Char to search.
 * @return          Pointer to start if search successfully.
 */
cf_char_t* cf_strchr(const cf_char_t* s, cf_char_t c);

/**
 * Index of a character in reverse order.
 * @param s         Source string to search into.
 * @param c         Char to search.
 * @return          Pointer to start if search successfully.
 */
cf_char_t* cf_strrchr(const cf_char_t* s, cf_char_t c);

/**
 * snprintf
 * @param dst         Destionation address.
 * @param dstsize     Size of destination buffer.
 * @param format      format of string.
 * @return            print size.
 */
cf_int_t cf_snprintf(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* format, ...);

#define cf_str_startwith(s, search) (s == cf_strstr((s), (search)))
#define cf_str_endwith(s, search) ((s - cf_strlen(search)) == cf_strstr((s), search)))

CF_DECLS_END

#endif /* __CF_STR_H__ */
