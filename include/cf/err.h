#ifndef __CF_ERR_H__
#define __CF_ERR_H__

#include <cf/types.h>

/**
 * @defgroup CF_ERR errno
 * errno
 * @{
 *
 * errno: reserved(10bits) | mid(6bits) | errno(16bits)
 */

CF_DECLS_BEGIN


/** a custom errno should implement this function to allow strerr return the errno description */
typedef const cf_char_t*(*cf_strerr_fn)(cf_errno_t eid);

#define CF_ERR_CODE_BITS            16  /** use 16bits to hold errno */
#define CF_ERR_MID_BITS             6   /** use 5bits to hold mid */
#define CF_ERR_MID_START            0   /** start module ID */
#define CF_ERR_MID_NUM_MAX          (1 << CF_ERR_MID_BITS)  /** number of modules supportted */ 
#define CF_ERR_CODE_NUM_MAX         (1 << CF_ERR_CODE_BITS)

#define CF_ERR_CODE_MASK            (CF_ERR_CODE_NUM_MAX - 1)
#define CF_ERR_MID_MASK            (CF_ERR_MID_NUM_MAX - 1)
#define CF_MAKE_ERRNO(m, no)        ((m) << CF_ERR_CODE_BITS | no)
#define CF_ERR_GET_MODULE(err)      (((err) >> CF_ERR_CODE_BITS) & CF_ERR_MID_MASK)
#define CF_ERR_GET_MERR(err)        ((err) & CF_ERR_CODE_MASK)
#define CF_ERR_STR_BUILD(code, desc) {code, desc " (" #code ")"}


#define CF_EOK          CF_MAKE_ERRNO(CF_ERR_MID_START, 0)
#define CF_ENOK         CF_MAKE_ERRNO(CF_ERR_MID_START, 1)
#define CF_EPARAM       CF_MAKE_ERRNO(CF_ERR_MID_START, 2)
#define CF_ENULLPTR     CF_MAKE_ERRNO(CF_ERR_MID_START, 3)
#define CF_ENOTFOUND    CF_MAKE_ERRNO(CF_ERR_MID_START, 4)
#define CF_EFULL        CF_MAKE_ERRNO(CF_ERR_MID_START, 5)
#define CF_EEMPTY       CF_MAKE_ERRNO(CF_ERR_MID_START, 6)
#define CF_ETOOBIG      CF_MAKE_ERRNO(CF_ERR_MID_START, 7)
#define CF_ETOOSHORT    CF_MAKE_ERRNO(CF_ERR_MID_START, 8)
#define CF_EFOPEN       CF_MAKE_ERRNO(CF_ERR_MID_START, 9)
#define CF_EFCLOSE      CF_MAKE_ERRNO(CF_ERR_MID_START, 10)
#define CF_EFWRITE      CF_MAKE_ERRNO(CF_ERR_MID_START, 11)
#define CF_EEOF         CF_MAKE_ERRNO(CF_ERR_MID_START, 12)
#define CF_EFREAD       CF_MAKE_ERRNO(CF_ERR_MID_START, 13)
#define CF_EMALLOC      CF_MAKE_ERRNO(CF_ERR_MID_START, 14)
#define CF_EMFREE       CF_MAKE_ERRNO(CF_ERR_MID_START, 15)
#define CF_EDSO_OPEN    CF_MAKE_ERRNO(CF_ERR_MID_START, 16)
#define CF_EDSO_GETSYM  CF_MAKE_ERRNO(CF_ERR_MID_START, 17)
#define CF_ESELECT_TOUT CF_MAKE_ERRNO(CF_ERR_MID_START, 18)
#define CF_EBAD_ID      CF_MAKE_ERRNO(CF_ERR_MID_START, 19)

#define CF_OK   CF_EOK
#define CF_NOK  CF_ENOK

/**
 * Register error info to global config.
 * @param module        Module number.
 * @param fn            strerr function.
 * @param reg ok
 */
cf_errno_t cf_err_register(cf_uint32_t module, cf_strerr_fn fn);

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
