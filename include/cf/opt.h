#ifndef __CF_OPT_H__
#define __CF_OPT_H__

#include <cf/types.h>
#include <cf/string.h>

#define CF_OPT_ID_CMD_SELF  0
#define CF_OPT_ID_INVALID 0xFFFF
#define CF_OPT_NAME_MAX_SIZE 64
#define CF_OPT_ARG_MAX_SIZE 128

#define CF_OPT_ESSENTIAL_ARG_ESSENTIAL 3
#define CF_OPT_ESSENTIAL_ARG_OPTIONAL  2
#define CF_OPT_OPTIONAL_ARG_ESSENTIAL  1
#define CF_OPT_OPTIONAL_ARG_OPTIONAL   0

#define CF_OPT_ARG_ESSENTIAL(flag) ((flag) & 0x1)

typedef struct {
    cf_bool_t is_multi;
    cf_bool_t is_short;
    cf_char_t name[CF_OPT_NAME_MAX_SIZE];
    cf_bool_t has_arg;
    cf_char_t arg[CF_OPT_ARG_MAX_SIZE];
} cf_opt_t;

cf_errno_t cf_opt_get_raw(cf_uint_t argc, cf_char_t* argv[], cf_uint_t* pos, cf_opt_t* opt);

#endif /* __CF_OPT_H__ */
