#ifndef __CF_STRING_H__
#define __CF_STRING_H__
#include "cf_def.h"

typedef struct {
    cf_char_t*  data;   /** string data without \0 */
    cf_size_t   length; /** string length */
} cf_string_t;

typedef struct {
    const cf_char_t*    data;   /** string data without \0 */
    cf_size_t           length; /** string length */
} cf_const_string_t;


#endif /* __CF_STRING_H__ */