#ifndef __CF_OPT_H__
#define __CF_OPT_H__

#include <cf/types.h>
#include <cf/string.h>

#define CF_OPT_ARG_MAX_SIZE 128

typedef struct cf_opt_s {
    cf_id_t             id;             /* id */
    cf_char_t           sname;          /* short name */
    const cf_char_t*    lname;          /* long name */
    cf_bool_t           essential;      /* option is essential */
    cf_bool_t           arg_essential;  /* arg of this option is essential */
} cf_opt_t;

typedef struct cf_opt_it_s {
    const cf_opt_t* opt;
    cf_uint_t       pos;
    cf_char_t       arg[CF_OPT_ARG_MAX_SIZE];
} cf_opt_it_t;

cf_errno_t cf_opt_get(cf_uint_t argc, cf_char_t* argv[], const cf_opt_t* optdef, cf_opt_it_t* it);


#endif /* __CF_OPT_H__ */
