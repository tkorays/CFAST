#ifndef __CF_OPT_IF_H__
#define __CF_OPT_IF_H__

#include "cf_def.h"
#include "cf_list_if.h"

/**
 * The structure represents an option.
 */
typedef struct {
    cf_char_t   short_name;     /** option's short name */
    cf_char_t   long_name[16];  /** option's long name */
    cf_bool_t   has_arg;        /** does this option have arguement */
    cf_char_t*  arg;            /** arguement of this option */
} cf_opt_t;


cf_errno_t cf_opt_get(cf_uint32_t argc, cf_char_t** argv, cf_list_t* opts);

#endif /* __CF_OPT_IF_H__ */
