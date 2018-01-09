#include "cfast/cf_opt_if.h"
#include "cfast/cf_mem_if.h"

#include <string.h>

cf_errno_t cf_opt_get(cf_uint32_t argc, cf_char_t** argv, cf_opt_t** opts)
{
    cf_uint32_t i;
    cf_bool_t need_param = CF_FALSE;
    if(argc == 0 || !argv || !opts) return CF_ERRNO_INVALID_PARAM;

    *opts = (cf_opt_t*)cf_malloc(sizeof(cf_opt_t) * argc);
    if(!*opts) return CF_ERRNO_MALLOC_ERROR;
    (cf_void_t)cf_memset_s(*opts, sizeof(cf_opt_t) * argc, 0, sizeof(cf_opt_t) * argc);

    for(i = 0; i < argc && argv[i]; i++) {

    }
    

    return CF_ERRNO_OK;
}