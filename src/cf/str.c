#include <cf/str.h>
#include <cf/memory.h>
#include <stdio.h>
#include <stdarg.h>
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

cf_int32_t cf_stricmp(const cf_char_t* s1, const cf_char_t* s2) {
    for(; CF_TO_LOWER_ALPHA(*s1) == CF_TO_LOWER_ALPHA(*s2); ++s1,++s2) if(*s1 == '\0') return 0;
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

cf_char_t* cf_strncpy_s(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* src, cf_size_t srcmax) {
    cf_size_t srcsize;
    if(!dst || !src) return CF_NULL_PTR;

    srcsize = cf_strlen(src) + 1;
    srcmax = srcmax > srcsize ? srcmax : srcsize;

    if(srcsize > dstsize) return CF_NULL_PTR;
    if(!cf_memcpy_s(dst, dstsize, src, srcsize)) return CF_NULL_PTR;

    dst[srcsize - 1] = '\0';
    return dst;   
}

cf_char_t* cf_strstr(cf_char_t* s1, cf_char_t* s2) {
    return (cf_char_t*)strstr(s1, s2);
}

cf_char_t* cf_strrstr(cf_char_t* s1, cf_char_t* s2) {
    return CF_NULL_PTR;
}

cf_char_t* cf_strchr(const cf_char_t* s, cf_char_t c) {
    if(!s) return CF_NULL_PTR;
    while(*s && *s != c) s++;
    return (*s ? (cf_char_t*)s : CF_NULL_PTR);
}

cf_char_t* cf_strrchr(const cf_char_t* s, cf_char_t c) {
    cf_size_t n;
    if(!s) return CF_NULL_PTR;
    n = cf_strlen(s);
    s += n;
    while(n > 0 && *s != c) n--, s--;
    return (n < 0 ? CF_NULL_PTR : (cf_char_t*)s);
}

cf_int_t cf_snprintf(cf_char_t* dst, cf_size_t dstsize, const cf_char_t* format, ...) {
    va_list args;
    int ret;
#ifdef CF_OS_WIN
#define vsnprintf _vsnprintf
#endif
    va_start(args, format);
    ret = vsnprintf(dst, dstsize, format, args);
    va_end(args);
    return ret;
}

cf_bool_t cf_str_strip(cf_char_t* s) {
    cf_char_t* start = CF_NULL_PTR;
    cf_char_t* end = CF_NULL_PTR;
    cf_size_t len = 0;
    cf_size_t i = 0;
    if (!s) return CF_FALSE;

    len = strlen(s);
    if (len == 0) return CF_TRUE;

    /* strip left */
    start = s;
    while (CF_IS_SPACE(*start)) {
        start++;
    }
    // all chars are \b or \t
    if (*start == '\0') {
        s[0] = '\0';
        return CF_TRUE;
    }

    /* strip right */
    end = s + len - 1;
    while (CF_IS_SPACE(*end)) {
        end--;
    }

    len = end - start + 1;    
    for (i = 0; i < len; i++) {
        s[i] = *(start + i);
    }
    s[len] = '\0';
    return CF_TRUE;
}

cf_bool_t cf_str_capitalize(cf_char_t* s) {
    if (!s) return CF_FALSE;
    if (strlen(s) == 0) return CF_TRUE;
    // is upper already
    s[0] = CF_TO_UPPER_ALPHA(s[0]);
    return CF_TRUE;
}

cf_bool_t cf_str_to_upper(cf_char_t* s) {
    cf_char_t* p = CF_NULL_PTR;
    if (!s) return CF_FALSE;

    for (p = s; *p != '\0'; p++) {
        *p = CF_TO_UPPER_ALPHA(*p);
    }
    return CF_TRUE;
}

cf_bool_t cf_str_to_lower(cf_char_t* s) {
    cf_char_t* p = CF_NULL_PTR;
    if (!s) return CF_FALSE;

    for (p = s; *p != '\0'; p++) {
        *p = CF_TO_LOWER_ALPHA(*p);
    }
    return CF_TRUE;
}

cf_bool_t cf_str_switch_case(cf_char_t* s) {
    cf_char_t* p = CF_NULL_PTR;
    if (!s) return CF_FALSE;

    for (p = s; *p != '\0'; p++) {
        if (CF_IS_LOWER_ALPHA(*p)) {
            *p = CF_TO_UPPER_ALPHA(*p);
        } else {
            *p = CF_TO_LOWER_ALPHA(*p);
        }
    }
    return CF_TRUE;
}

cf_bool_t cf_str_center(cf_char_t* dst, cf_size_t size, cf_char_t* s, cf_char_t c, cf_size_t total) {
    cf_size_t left = 0, right = 0, len = 0;
    cf_char_t* p = CF_NULL_PTR;
    if (!dst || !s) {
        return CF_FALSE;
    }
    // no enough space to hold 
    len = strlen(s);
    if (len + 1 > size || total + 1 > size) {
        return CF_FALSE;
    }

    left = (total - len) / 2;
    p = dst;
    // fill left
    while (p < dst + left) {
        *p = c;
        p++;
    }
    // copy string
    while (*s != '\0') {
        *p = *s;
        s++;
        p++;
    }
    // fill right
    while (p < dst + total) {
        *p = c;
        p++;
    }
    dst[total] = '\0';
    
    return CF_TRUE;
}

cf_size_t cf_str_count_for(cf_char_t* s, cf_char_t c) {
    cf_size_t cnt = 0;

    if (!s) return 0;
    if (c == '\0') return 1;

    while (*s != '\0') {
        if (*s == c) cnt ++;
        s++;
    }
    return cnt;
}

