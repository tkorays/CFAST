#include <cf/opt.h>
#include <cf/memory.h>
#include <cf/str.h>
#include <cf/err.h>

#include <string.h>


#define CF_OPT_BE_PARAM(s) (cf_strlen((s)) > 0 && ((s)[0] != '-'))

static const cf_opt_t* __cf_opt_find_opt_by_short_name(const cf_opt_t* optdef, cf_char_t c) {
    cf_uint_t i;
    if(!optdef) return CF_NULL_PTR;
    for(i = 0; ; i++) {
        if(optdef[i].id == CF_OPT_ID_INVALID) break;
        if(optdef[i].sname == c) return &optdef[i];
    }
    return CF_NULL_PTR;
}

 cf_errno_t cf_opt_get(cf_uint_t argc, cf_char_t* argv[], const cf_opt_t* optdef, cf_opt_it_t* it) {
     cf_uint_t i;
     const cf_opt_t* opt = CF_NULL_PTR;
     if(argc == 0 || argv == CF_NULL_PTR || optdef == CF_NULL_PTR || it == CF_NULL_PTR) {
         return CF_EPARAM;
     }

     if(it->argi >= argc) return CF_EEOF;
     // 为空，非法
     if(!argv[it->argi]) return CF_EEOF;
     // 长度为0，非法
     if(cf_strlen(argv[it->argi]) == 0) return CF_EEOF;

     if(it->start == 0) {
         if (argv[it->argi][0] == '-' && argv[it->argi][1] != '-' && argv[it->argi][1] != '\0') {
             // short name
             opt = __cf_opt_find_opt_by_short_name(optdef, argv[it->argi][1]);
             if (!opt) return CF_ENOTFOUND;

             it->opt = opt;

             // 参数必需
             if (CF_OPT_ARG_ESSENTIAL(opt->flag)) {

                 if ((it->argi + 1) >= argc) return CF_NOK;

                 if (CF_OPT_BE_PARAM(argv[it->argi])) {
                     cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                     it->hasarg = CF_TRUE;
                     it->argi++;
                     return CF_OK;
                 }
                 return CF_NOK; // 后面一个必需为参数
             }

             // 组合参数情况下
             if (argv[it->argi][2] != '\0') {
                 it->hasarg = CF_FALSE;
                 it->start = 2;
                 return CF_OK;
             }

             // 可选参数
             if ((it->argi + 1) < argc) {
                 if (CF_OPT_BE_PARAM(argv[it->argi])) {
                     cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                     it->hasarg = CF_TRUE;
                     it->argi++;
                     return CF_OK;
                 }
             }
         }
     }
     return CF_OK;
 }