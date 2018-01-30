#include <cf/opt.h>
#include <cf/memory.h>
#include <cf/str.h>
#include <cf/err.h>

#define CF_OPT_BE_PARAM(s) (cf_strlen((s)) > 0 && ((s)[0] != '-'))
#define CF_OPT_BE_SHORT_OPT(s) (cf_strlen(s) == 2 && (s)[0] == '-' && (s)[1] != '-')
#define CF_OPT_BE_LONG_OPT(s) (cf_strlen(s) > 2 && (s)[0] == '-' && (s)[1] == '-' && (s)[2] != '-')
#define CF_OPT_BE_MULTI_OPT(s) (cf_strlen(s) > 2 && (s)[0] == '-' && (s)[1] != '-')

cf_errno_t cf_opt_get_raw(cf_uint_t argc, cf_char_t* argv[], cf_uint_t* pos, cf_opt_t* opt) {
    cf_char_t *p;
    if(argc == 0 || !argv || !opt || !pos) return CF_NOK;
    opt->is_multi = CF_FALSE;
    opt->is_short = CF_FALSE;

    if(*pos >= argc) return CF_EEOF;
    if(argc <= 1) return CF_NOK;
    if(*pos == 0) *pos = 1;
    if(cf_strlen(argv[*pos]) >= CF_OPT_MAX_SIZE) return CF_NOK;
    cf_memset_s(opt, sizeof(cf_opt_t), 0, sizeof(cf_opt_t));

    if(CF_OPT_BE_SHORT_OPT(argv[*pos])) {
        opt->is_multi = CF_FALSE;
        opt->is_short = CF_TRUE;
        opt->name[0] = argv[*pos][1];
        if((*pos + 1) < argc && CF_OPT_BE_PARAM(argv[*pos + 1])) {
            opt->has_arg = CF_TRUE;
            cf_strcpy_s(opt->arg, sizeof(opt->arg), argv[*pos + 1]);
            (*pos)++;
        } else opt->has_arg = CF_FALSE;
    } else if(CF_OPT_BE_LONG_OPT(argv[*pos])) {
        opt->is_multi = CF_FALSE;
        opt->is_short = CF_FALSE;
        p = cf_strchr(argv[*pos], '=');
        if(p) {
            cf_memcpy_s(opt->name, sizeof(opt->name), argv[*pos], (p - argv[*pos]));
            cf_strcpy_s(opt->arg, sizeof(opt->arg), p + 1);
            opt->has_arg = CF_TRUE;
        } else {
            cf_strcpy_s(opt->name, sizeof(opt->name), &argv[*pos][2]);
            opt->has_arg = CF_FALSE;
        }
    } else if(CF_OPT_BE_MULTI_OPT(argv[*pos])) {
        opt->is_multi = CF_TRUE;
        opt->is_short = CF_TRUE;
        opt->has_arg = CF_FALSE;
        cf_strcpy_s(opt->name, sizeof(opt->name), &argv[*pos][1]);
    } else {
        return CF_NOK;
    }

    (*pos)++;
    return CF_OK;
}
