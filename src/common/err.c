#include <cf/err.h>
#include <cf/str.h>

/** 各个模块需要注册错误码描述 */
cf_err_pfn_strerror g_err4modules[CF_ERR_MODULE_MAX_NUM] = {0};

typedef struct {
    cf_size_t   size;
    const cf_err_info_t* errs;
} _cf_errors_in_module_t;

static cf_err_info_t g_common_err_desc[] = {
    {CF_OK, "ok"},
    {CF_NOK, "error!"},
    {CF_EPARAM, "Paramenters error!"},
    {CF_EMALLOC, "Memory allocation error!"},
    {CF_EFOPEN, "Fail to open file!"},
    {CF_EFCLOSE, "Fail to close file!"},
    {CF_EFILE_HANDLE, "File handle is error!"},
    {CF_EFWRITE, "Fail to write file."},
    {CF_EFREAD, "Fail to read file."},
    {CF_ENULLPTR, "Null pointer."},
    {CF_EEOF, "End of file."},
    {CF_EFULL, "Full!"},
    {CF_EEMPTY, "Empty!"},
    {CF_ETOOBIG, "Size is too big!"},
    {CF_ETOOSHORT, "Size is too short!"},
    {CF_ENOTFOUND, "Not Found!"}
};

_cf_errors_in_module_t g_err_desc[CF_ERR_MODULE_MAX_NUM] = {
    {CF_ARRAY_SIZE(g_common_err_desc), g_common_err_desc},
    {0, CF_NULL_PTR}
};

cf_void_t cf_err_get(cf_errno_t err, cf_char_t* desc, cf_size_t size) {
    cf_uint32_t m = CF_ERR_GET_MODULE(err);
    cf_uint32_t e = CF_ERR_GET_MERR(err);
    if(!desc || size == 0) {
        cf_strcpy_s(desc, size, "Invalid param for cf_err_get_info!");
        return;
    }
    if(m >= CF_ERR_MODULE_MAX_NUM) {
        cf_strcpy_s(desc, size, "Errno error for cf_err_get_info!");
        return ;
    }
    cf_strcpy_s(desc, size, g_err_desc[m].errs[e].desc);
}

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