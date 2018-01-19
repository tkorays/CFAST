#include <cf/err.h>
#include <cf/str.h>

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
    {CF_EEMPTY, "Empty!"}
};

_cf_errors_in_module_t g_err_desc[CF_ERR_MODULE_MAX_NUM] = {
    {CF_ARRAY_SIZE(g_common_err_desc), g_common_err_desc},
    {0, CF_NULL_PTR}
};

cf_void_t cf_err_get_info(cf_errno_t err, cf_char_t* desc, cf_size_t size) {
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

cf_void_t cf_err_register(cf_uint32_t module, cf_err_info_t* errs, cf_size_t size) {

}