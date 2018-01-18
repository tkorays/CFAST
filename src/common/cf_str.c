#include "cfast/cf_str_if.h"
#include "cfast/cf_mem_if.h"
#include "cfast/cf_err_if.h"

#include <string.h>

cf_size_t cf_strlen(const cf_char_t* s) {
    const cf_char_t *p = s;
    while(*p != '\0') p++;
    return (p - s);
}

cf_int32_t cf_strcmp(const cf_char_t* s1, const cf_char_t* s2) {
    for(; *s1 == *s2; ++s1,++s2) if(*s1 == '\0') return 0;
    return (*(cf_uchar_t*)s1 < *(cf_uchar_t*)s2 ? -1 : 1);
}

cf_int32_t cf_strcasecmp(const cf_char_t* s1, const cf_char_t* s2) {
    for(;; ++s1, ++s2) {
        if(*s1 == '\0' && *s2 == '\0') return 0;
        if(*s1 == *s2) continue;
        if(*(cf_uchar_t*)s1 >= 'a' && *(cf_uchar_t*)s1 <= 'z' 
            && (*(cf_uchar_t*)s1 + 32) == *(cf_uchar_t*)s2) continue; 
        if(*(cf_uchar_t*)s1 >= 'A' && *(cf_uchar_t*)s1 <= 'Z' 
            && (*(cf_uchar_t*)s1 - 32) == *(cf_uchar_t*)s2) continue; 
    }
    return (*(cf_uchar_t*)s1 < *(cf_uchar_t*)s2 ? -1 : 1);
}
cf_char_t* cf_strcat_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src) {
    cf_size_t srcsize;
    if(!dst || !src) return CF_NULL_PTR;

    srcsize = cf_strlen(src) + 1;
    if(srcsize > dstsize) return CF_NULL_PTR;
    
    return (cf_char_t*)strcat(dst, src);
}

cf_char_t* cf_strcpy_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src) {
    cf_size_t srcsize;
    if(!dst || !src) return CF_NULL_PTR;

    srcsize = cf_strlen(src) + 1;
    if(srcsize > dstsize) return CF_NULL_PTR;
    if(!cf_memcpy_s(dst, dstsize, src, srcsize)) return CF_NULL_PTR;
    return dst;
}

cf_char_t* cf_strstr(cf_char_t* s1, cf_char_t* s2) {
    return (cf_char_t*)strstr(s1, s2);
}

cf_char_t* cf_strrstr(cf_char_t* s1, cf_char_t* s2) {
    return CF_NULL_PTR;
}

cf_char_t* cf_strchr(const cf_char_t* s, cf_char_t c) {
    return (cf_char_t*)strchr(s, c);
}
