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

typedef struct cf_opt_s {
    cf_id_t             id;             /* id */
    cf_char_t           sname;          /* short name */
    const cf_char_t*    lname;          /* long name */
    cf_uint_t           flag;           /* flag of opt */
} cf_opt_t;

typedef struct cf_opt_it_s {
    const cf_opt_t* opt;
    cf_uint_t       argi;   /* index of arg */
    cf_uint_t       start;  /* char start, start != 0 if opt is multi arg */
    cf_bool_t       hasarg;
    cf_char_t       arg[CF_OPT_ARG_MAX_SIZE];
} cf_opt_it_t;

cf_errno_t cf_opt_get(cf_uint_t argc, cf_char_t* argv[], const cf_opt_t* optdef, cf_opt_it_t* it);


#endif /* __CF_OPT_H__ */
