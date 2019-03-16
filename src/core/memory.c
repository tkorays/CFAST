#include <cf/memory.h>
#include <stdlib.h>
#include <string.h>

#define CF_MEMTRACE_MALLOC(p, size) (void*)0
#define CF_MEMTRACE_FREE(p) (void*)0

cf_void_t* cf_malloc_dbg(cf_size_t size) {
    cf_void_t* p = malloc(size);

    CF_MEMTRACE_MALLOC(p, size);
    return p;
}

cf_void_t  cf_free_dbg(cf_void_t* addr) {
    free(addr);
    
    CF_MEMTRACE_FREE(addr);
}

cf_void_t* cf_realloc_dbg(cf_void_t* addr, cf_size_t size) {
    return realloc(addr, size);
}

cf_void_t* cf_memcpy_s(cf_void_t* dst, cf_size_t dst_size, const cf_void_t* src, cf_size_t src_size) {
    if(dst_size < src_size) return CF_NULL_PTR;
    return memcpy(dst, src, src_size);
}

cf_void_t* cf_memset_s(cf_void_t* s, cf_size_t smax, cf_char_t c, cf_size_t rsize) {
    if(smax < rsize) return CF_NULL_PTR;
    return memset(s, c, rsize);
}

cf_void_t* cf_memchr(const cf_void_t* s, cf_char_t c, cf_size_t n) {
    cf_char_t* ss = (cf_char_t*)s;
    if(!s || !n) return CF_NULL_PTR;
    while(n > 0 && *ss != c) ss++, n--;
    return (n == 0 ? CF_NULL_PTR : ss);
}

cf_void_t cf_membzero(cf_void_t* dst, cf_size_t n) {
#ifdef __GLIBC__
    bzero(dst, n);
#else
    cf_memset_s(dst, n, 0, n);
#endif
}
