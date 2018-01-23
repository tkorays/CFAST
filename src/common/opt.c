#include <cf/opt.h>
#include <cf/memory.h>
#include <cf/err.h>

#include <string.h>
 
 cf_errno_t cf_opt_get(cf_uint_t argc, cf_char_t* argv[], const cf_opt_t* optdef, cf_opt_it_t* it) {
     cf_uint_t i;
     if(argc == 0 || argv == CF_NULL_PTR || optdef == CF_NULL_PTR || it == CF_NULL_PTR) {
         return CF_EPARAM;
     }
     if(it->pos >= argc) return CF_EEOF;

     //argv[it->pos];
     return CF_OK;
 }