#ifndef __CF_ERR_H__
#define __CF_ERR_H__

#include <cf/types.h>

/**
 * @defgroup CF_ERR error
 * error
 * @{
 */

CF_DECLS_BEGIN


typedef const cf_char_t*(*cf_strerr_fn)(cf_uint_t eid);

/**
 * module id should start at 100.
 */
#define CF_ERR_MODULE_START         100

/**
 * we can only define 16 modules.
 * you can modify this value to support more modules.
 */
#define CF_ERR_MODULE_MAX_NUM       16

/**
 * err code has 16 bits.
 */
#define CF_ERR_CODE_BITS            16

/**
 * module has 16 bits.
 */
#define CF_ERR_MODULE_BITS          16
#define CF_ERR_CODE_MASK            0x00FFFF
#define CF_MAKE_ERRNO(m, no)        ((m) << CF_ERR_CODE_BITS | no)
#define CF_ERR_GET_MODULE(err)      (((err) >> CF_ERR_CODE_BITS) & CF_ERR_CODE_MASK)
#define CF_ERR_GET_MERR(err)        ((err) & CF_ERR_CODE_MASK)
#define CF_ERR_DESC_MAX_BUFF        128
#define CF_ERR_STR_BUILD(code, desc) {code, desc " (" #code ")"}


#define CF_EOK          CF_MAKE_ERRNO(CF_ERR_MODULE_START, 0)
#define CF_ENOK         CF_MAKE_ERRNO(CF_ERR_MODULE_START, 1)
#define CF_EPARAM       CF_MAKE_ERRNO(CF_ERR_MODULE_START, 2)
#define CF_ENULLPTR     CF_MAKE_ERRNO(CF_ERR_MODULE_START, 3)
#define CF_ENOTFOUND    CF_MAKE_ERRNO(CF_ERR_MODULE_START, 4)
#define CF_EFULL        CF_MAKE_ERRNO(CF_ERR_MODULE_START, 5)
#define CF_EEMPTY       CF_MAKE_ERRNO(CF_ERR_MODULE_START, 6)
#define CF_ETOOBIG      CF_MAKE_ERRNO(CF_ERR_MODULE_START, 7)
#define CF_ETOOSHORT    CF_MAKE_ERRNO(CF_ERR_MODULE_START, 8)
#define CF_EFOPEN       CF_MAKE_ERRNO(CF_ERR_MODULE_START, 9)
#define CF_EFCLOSE      CF_MAKE_ERRNO(CF_ERR_MODULE_START, 10)
#define CF_EFWRITE      CF_MAKE_ERRNO(CF_ERR_MODULE_START, 11)
#define CF_EEOF         CF_MAKE_ERRNO(CF_ERR_MODULE_START, 12)
#define CF_EFREAD       CF_MAKE_ERRNO(CF_ERR_MODULE_START, 13)
#define CF_EMALLOC      CF_MAKE_ERRNO(CF_ERR_MODULE_START, 14)
#define CF_EMFREE       CF_MAKE_ERRNO(CF_ERR_MODULE_START, 15)
#define CF_EDSO_OPEN    CF_MAKE_ERRNO(CF_ERR_MODULE_START, 16)
#define CF_EDSO_GETSYM  CF_MAKE_ERRNO(CF_ERR_MODULE_START, 17)
#define CF_ESELECT_TOUT CF_MAKE_ERRNO(CF_ERR_MODULE_START, 18)

#define CF_OK   CF_EOK
#define CF_NOK  CF_ENOK

/**
 * Register error info to global config.
 * @param module        Module number.
 * @param fn            strerr function.
 * @param reg ok
 */
cf_void_t cf_err_register(cf_uint32_t module, cf_strerr_fn fn);

/**
 * strerror.
 * @param err           errno.
 * @param msg           error message.
 * @param size          buff size.
 */
cf_void_t cf_err_strerr(cf_errno_t err, cf_char_t* msg, cf_size_t size);

CF_DECLS_END

/** @} */

#endif /* __CF_ERR_H__ */
