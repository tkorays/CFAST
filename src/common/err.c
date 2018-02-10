#include <cf/err.h>
#include <cf/str.h>

const cf_char_t* __strerror(cf_uint_t eid, cf_err_info_t* ast, cf_size_t size) {
    cf_uint_t i;
    for (i = 0; i < size; ++i) {
        if(eid == CF_ERR_GET_MERR(ast[i].err)) return ast[i].desc;
    }
    return "Errno NOT FOUND";
}

cf_err_info_t common_err_str[] = {
    CF_ERR_STR_BUILD(CF_EOK, "OK."),
    CF_ERR_STR_BUILD(CF_ENOK, "Not OK."),
    CF_ERR_STR_BUILD(CF_EPARAM, "Invalid params."),
    CF_ERR_STR_BUILD(CF_ENULLPTR, "Pointer is NULL."),
    CF_ERR_STR_BUILD(CF_ENOTFOUND, "Not found."),
    CF_ERR_STR_BUILD(CF_EFULL, "Full."),
    CF_ERR_STR_BUILD(CF_EEMPTY, "Empty"),
    CF_ERR_STR_BUILD(CF_ETOOBIG, "Too big."),
    CF_ERR_STR_BUILD(CF_ETOOSHORT, "Too short.")
};

const cf_char_t* __common_strerr(cf_uint_t eid) {
    return __strerror(eid, common_err_str, CF_ARRAY_SIZE(common_err_str));
}


cf_err_info_t file_err_str[] = {
    CF_ERR_STR_BUILD(CF_EFOPEN, "fopen error."),
    CF_ERR_STR_BUILD(CF_EFCLOSE, "fclose error."),
    CF_ERR_STR_BUILD(CF_EFWRITE, "fwrite error."),
    CF_ERR_STR_BUILD(CF_EEOF, "end of file."),
    CF_ERR_STR_BUILD(CF_EFREAD, "fread error."),
};

const cf_char_t* __file_strerr(cf_uint_t eid) {
    return __strerror(eid, file_err_str, CF_ARRAY_SIZE(file_err_str));
}

cf_err_info_t mem_err_str[] = {
        CF_ERR_STR_BUILD(CF_EMALLOC, "Memory allocate error."),
        CF_ERR_STR_BUILD(CF_EMFREE, "Memory free error.")
};

const cf_char_t* __mem_strerr(cf_uint_t eid) {
    return __strerror(eid, mem_err_str, CF_ARRAY_SIZE(mem_err_str));
}

/** 各个模块需要注册错误码描述 */
static cf_err_pfn_strerror g_err4modules[CF_ERR_MODULE_MAX_NUM] = {
    __common_strerr,
    __file_strerr,
    __mem_strerr,
    0
};


cf_void_t cf_err_register(cf_uint32_t module, cf_err_pfn_strerror fn) {
    if(module >= CF_ERR_MODULE_MAX_NUM || !fn) return ;
    g_err4modules[module] = fn;
    return ;
}

cf_void_t cf_err_strerr(cf_errno_t err, cf_char_t* msg, cf_size_t size) {
    cf_uint_t m = (cf_uint_t)CF_ERR_GET_MODULE(err);
    cf_uint_t e = (cf_uint_t)CF_ERR_GET_MERR(err);

    if(!msg || size == 0) {
        cf_strcpy_s(msg, size, "Params' ERROR for cf_err_strerr!");
        return;
    }

    if(m >= CF_ERR_MODULE_MAX_NUM) {
        cf_strcpy_s(msg, size, "Module NOT FOUND!");
        return;
    }

    if(g_err4modules[m]) {
        cf_strcpy_s(msg, size, g_err4modules[m](e));
    } else {
        cf_strcpy_s(msg, size, "Module NOT REGISTERED!");
    }
}