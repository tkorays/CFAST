#ifndef __CF_OPT_IF_H__
#define __CF_OPT_IF_H__

#include "cf_def.h"
#include "cf_string_if.h"


/**
 * The structure represents an option.
 * It will not allocate memory or copy memory.
 * cfg_opt_t just use pointer to point to real address.
 */
typedef struct {
    cf_bool_t           is_short;       /** is short name or not */
    cf_string_const_t   name;           /** name, don't change this data */
    cf_bool_t           has_arg;        /** does this option have arguement */
    cf_string_const_t   arg;            /** arguement of this option, don't change */
} cf_opt_t;


cf_errno_t cf_opt_get(cf_uint32_t argc, cf_char_t** argv, cf_opt_t** opts);

#endif /* __CF_OPT_IF_H__ */
