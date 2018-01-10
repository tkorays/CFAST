#include "cfast/cf_string_if.h"
#include "cfast/cf_mem_if.h"
#include <string.h>

cf_string_t* cf_str_new(cf_size_t size) {
    cf_string_t* s = CF_NULL_PTR;
    if(size == 0) return CF_NULL_PTR;

    s = (cf_string_t*)cf_malloc(sizeof(cf_string_t));
    if(!s) return CF_NULL_PTR;

    s->length = size;
    return s;
}

cf_void_t cf_str_free(cf_string_t* s) {
    if(!s) return;
    if(s->data) cf_free(s->data);
    cf_free(s);
}

cf_void_t cf_str_cont_free(cf_string_const_t* s) {
    if(!s) return;
    cf_free(s);
}

cf_string_t* cf_str_from_pchar(cf_char_t* pc) {
    cf_string_t* s = CF_NULL_PTR;
    if(!pc) return CF_NULL_PTR;

    s = (cf_string_t*)cf_malloc(sizeof(cf_string_t));
    if(!s) return CF_NULL_PTR;

    s->data = (cf_char_t*)cf_malloc(strlen(pc) + 1);
    cf_memcpy_s(s->data, strlen(pc) + 1, pc, strlen(pc) + 1);
    s->length = strlen(pc);
    return s;
}

cf_size_t cf_str_len(cf_string_t* s) {
    if(!s || !s->data) return 0;
    return s->length;
}

cf_void_t cf_str_to_pchar(cf_string_t*s, cf_char_t** ppc) {
    if(!s || !ppc) return ;

    *ppc = (cf_char_t*)cf_malloc(s->length+1);
    if(!*ppc) return;
    if(s->length) {
        cf_memcpy_s(*ppc, s->length+1, s->data, s->length+1);
    }
    (*ppc)[s->length] = '\0';
}
