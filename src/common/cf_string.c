#include "cfast/cf_string_if.h"
#include "cfast/cf_mem_if.h"
#include <string.h>

cf_string_t* cf_str_from_pchar(cf_char_t* pc) {
    cf_string_t* s = CF_NULL_PTR;
    if(!pc) return CF_NULL_PTR;

    s = (cf_string_t*)cf_malloc(sizeof(cf_string_t));
    if(!s) return CF_NULL_PTR;

    s->data = pc;
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
