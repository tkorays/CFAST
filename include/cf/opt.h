#ifndef __CF_OPT_H__
#define __CF_OPT_H__

#include <cf/types.h>

CF_DECLS_BEGIN

#define CF_OPT_NAME_MAX_SIZE 64
#define CF_OPT_ARG_MAX_SIZE 128
#define CF_OPT_MAX_SIZE 128


typedef struct {
    cf_bool_t is_multi;
    cf_bool_t is_short;
    cf_char_t name[CF_OPT_NAME_MAX_SIZE];
    cf_bool_t has_arg;
    cf_char_t arg[CF_OPT_ARG_MAX_SIZE];
} cf_opt_t;

cf_errno_t cf_opt_get_raw(cf_uint_t argc, cf_char_t* argv[], cf_uint_t* pos, cf_opt_t* opt);

CF_DECLS_END

#endif /* __CF_OPT_H__ */
