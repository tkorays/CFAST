#include <cf/memory.h>
#include <stdlib.h>
#include <string.h>

cf_void_t* cf_malloc(cf_size_t size) {
    return malloc(size);
}

cf_void_t  cf_free(cf_void_t* addr) {
    free(addr);
}

cf_void_t* cf_realloc(cf_void_t* addr, cf_size_t size) {
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
