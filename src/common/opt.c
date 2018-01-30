#include <cf/opt.h>
#include <cf/memory.h>
#include <cf/str.h>
#include <cf/err.h>
#include <stdio.h>
#include <string.h>


#define CF_OPT_BE_PARAM(s) (cf_strlen((s)) > 0 && ((s)[0] != '-'))
#define CF_OPT_BE_SHORT_OPT(s) (cf_strlen(s) == 2 && (s)[0] == '-' && (s)[1] != '-')
#define CF_OPT_BE_LONG_OPT(s) (cf_strlen(s) > 2 && (s)[0] == '-' && (s)[1] == '-' && (s)[2] != '-')
#define CF_OPT_BE_MULTI_OPT(s) (cf_strlen(s) > 2 && (s)[0] == '-' && (s)[1] != '-')

static const cf_opt_t* __cf_opt_find_opt_by_short_name(const cf_opt_t* optdef, cf_char_t c) {
    cf_uint_t i;
    if(!optdef) return CF_NULL_PTR;
    for(i = 0; ; i++) {
        if(optdef[i].id == CF_OPT_ID_INVALID) break;
        if(optdef[i].sname == c) return &optdef[i];
    }
    return CF_NULL_PTR;
}

static const cf_opt_t* __cf_opt_find_opt_by_long_name(const cf_opt_t* optdef, const cf_char_t* n) {
    cf_uint_t i;
    if(!optdef || !n) return CF_NULL_PTR;
    for(i = 0; ; i++) {
        if(optdef[i].id == CF_OPT_ID_INVALID) break;
        if(cf_strcmp(optdef[i].lname, n) == 0) return &optdef[i];
    }
    return CF_NULL_PTR;
}

typedef struct {
    cf_bool_t is_multi;
    cf_bool_t is_short;
    cf_char_t name[CF_OPT_NAME_MAX_SIZE];
    cf_bool_t has_arg;
    cf_char_t arg[CF_OPT_ARG_MAX_SIZE];
} cf_opt_t;

cf_errno_t cf_opt_get_raw(cf_uint_t argc, cf_char_t* argv[], cf_uint_t* pos, cf_opt_t* opt) {
    if(argc == 0 || !argv || !opt || !pos) return CF_NOK;
    opt->is_multi = CF_FALSE;
    opt->is_short = CF_FALSE;

    if(argc <= 1) return CF_NOK;
    if(*pos == 0) *pos = 1;
    cf_memset_s(opt, sizeof(cf_opt_t), 0, sizeof(cf_opt_t));

    if(CF_OPT_BE_SHORT_OPT(argv[*pos])) {
        opt->is_multi = CF_FALSE;
        opt->is_short = CF_TRUE;
        opt->name[0] = argv[*pos][1];
    } else if(CF_OPT_BE_LONG_OPT(argv[*pos])) {
        opt->is_multi = CF_FALSE;
        opt->is_short = CF_FALSE;
        cf_strcpy_s(opt->name, sizeof(opt->name), &argv[*pos][2]);
    } else if(CF_OPT_BE_MULTI_OPT(argv[*pos])) {

    } else {
        return CF_NOK;
    }

    (*pos)++;
    return CF_OK;
}

 cf_errno_t cf_opt_get(cf_uint_t argc, cf_char_t* argv[], const cf_opt_t* optdef, cf_opt_it_t* it) {
     cf_uint_t i;
     const cf_opt_t* opt = CF_NULL_PTR;
     cf_bool_t is_short_opt = CF_TRUE;
     cf_bool_t is_multi_opt = CF_FALSE;
     if(argc == 0 || argv == CF_NULL_PTR || optdef == CF_NULL_PTR || it == CF_NULL_PTR) {
         return CF_EPARAM;
     }

     if(it->argi >= argc) return CF_EEOF;
     // 为空，非法
     if(!argv[it->argi]) return CF_NOK;
     // 长度为0，非法
     if(cf_strlen(argv[it->argi]) < 1) return CF_NOK;

     it->hasarg = CF_FALSE;

     // 判断是short还是long
     // 函数调用时start不为0，则一定是组合short且无参数
     if(it->start == 0) {
         if(argv[it->argi][0] == '-') {
             if(argv[it->argi][1] == '\0') return CF_NOK;
             else if(argv[it->argi][1] == '-' && argv[it->argi][2] == '\0') {
                 return CF_NOK;
             }  else if(argv[it->argi][1] == '-') {
                 is_short_opt = CF_FALSE;
                 it->start = 2;
             } else if(argv[it->argi][1] != '-') {
                 is_short_opt = CF_TRUE;
                 it->start = 1;
                 is_multi_opt = (argv[it->argi][2] == '\0') ? CF_FALSE : CF_TRUE;
             }
         } else {
             if(it->argi == 0) {
                 it->argi++;
                 it->opt = &optdef[0];
                 return CF_OK;
             }
             return CF_NOK;
         }
     } else {
         is_short_opt = CF_TRUE;
         is_multi_opt = CF_TRUE;
     }

     if(is_short_opt) {
         opt = __cf_opt_find_opt_by_short_name(optdef, argv[it->argi][it->start]);
         if(!opt) return CF_ENOTFOUND;
         it->opt = opt;

         if(is_multi_opt) {
             if(CF_OPT_ARG_ESSENTIAL(opt->flag)) return CF_NOK;
             it->start++;
             if(argv[it->argi][it->start] == '\0') {
                 it->argi++;
                 it->start = 0;
             }
         } else {
             if(CF_OPT_ARG_ESSENTIAL(opt->flag)) {
                 // 需要参数
                 if ((it->argi + 1) >= argc) return CF_NOK;
                 if(!CF_OPT_BE_PARAM(argv[it->argi+1])) return CF_NOK;
                 else {
                     cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                     it->hasarg = CF_TRUE;
                     it->argi++;
                 }
             } else {
                 // 可选参数情况下，探测是否需要参数
                 if((it->argi + 1) < argc && CF_OPT_BE_PARAM(argv[it->argi+1])) {
                     cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                     it->hasarg = CF_TRUE;
                     it->argi++;
                 }
             }
             it->argi++;
             it->start = 0;
         }
     } else {
         opt = __cf_opt_find_opt_by_long_name(optdef, &argv[it->argi][it->start]);
         if(!opt) return CF_ENOTFOUND;
         it->opt = opt;

         if(CF_OPT_ARG_ESSENTIAL(opt->flag)) {
             // 需要参数
             if ((it->argi + 1) >= argc) return CF_NOK;
             if(!CF_OPT_BE_PARAM(argv[it->argi+1])) return CF_NOK;
             else {
                 cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                 it->hasarg = CF_TRUE;
                 it->argi++;
             }
         } else {
             // 可选参数情况下，探测是否需要参数
             if(CF_OPT_BE_PARAM(argv[it->argi+1])) {
                 cf_strcpy_s(it->arg, sizeof(it->arg), argv[it->argi + 1]);
                 it->hasarg = CF_TRUE;
                 it->argi++;
             }
         }
         it->argi++;
         it->start = 0;
     }
     return CF_OK;
 }