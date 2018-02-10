#ifndef __CF_ERR_H__
#define __CF_ERR_H__

#include <cf/types.h>

/**
 * Errno Description
 */
typedef struct {
    cf_errno_t          err;    /** errno */
    const cf_char_t*    desc;   /** description */
} cf_err_info_t;

typedef const cf_char_t*(*cf_err_pfn_strerror)(cf_uint_t eid);

#define CF_ERR_DESC_MAX_BUFF        128
#define CF_ERR_MODULE_MAX_NUM       16
#define CF_MAKE_ERRNO(m, no) ((m)<<16 | no)
#define CF_ERR_GET_MODULE(err) (((err)>>16) & 0x0000FFFF)
#define CF_ERR_GET_MERR(err) ((err) & 0x0000FFFF)
#define CF_OK           CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 0)
#define CF_NOK          CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 1)
#define CF_EPARAM       CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 2)
#define CF_EMALLOC      CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 3)
#define CF_EFOPEN       CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 4)
#define CF_EFCLOSE      CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 5)
#define CF_EFILE_HANDLE CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 6)
#define CF_EFWRITE      CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 7)
#define CF_EFREAD       CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 8)
#define CF_ENULLPTR     CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 9)
#define CF_EEOF         CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 10)
#define CF_EFULL        CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 11)
#define CF_EEMPTY       CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 12)
#define CF_ETOOBIG      CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 13)
#define CF_ETOOSHORT    CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 14)
#define CF_ENOTFOUND    CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 15)

/**
 * Get description of an error.
 * @param err           Errno of an error.
 * @param desc          Description buffer.
 * @param size          Size of 'desc' buffer.
 */
cf_void_t cf_err_get(cf_errno_t err, cf_char_t* desc, cf_size_t size);

/**
 * Register error info to global config.
 * @param module        Module number.
 * @param errs          Errors.
 * @param size          Size of 'errs'.
 * @param reg ok
 */
cf_void_t cf_err_register(cf_uint32_t module, cf_err_pfn_strerror fn);

/**
 * strerror.
 * @param err           errno.
 * @param msg           error message.
 * @param size          buff size.
 */
cf_void_t cf_err_strerr(cf_errno_t err, cf_char_t* msg, cf_size_t size);

#endif /* __CF_ERR_H__ */
