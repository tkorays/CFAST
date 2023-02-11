#ifndef __CF_SHM_H__
#define __CF_SHM_H__

#include <cf/types.h>

/**
 * @defgroup CF_SHM shared memory
 * shared memory
 * @{
 */

typedef struct cf_shm cf_shm_t;

CF_DECLS_BEGIN

CF_DECLARE(cf_errno_t) cf_shm_create(cf_shm_t** shm, cf_size_t size, const cf_char_t* filename);
CF_DECLARE(cf_errno_t) cf_shm_destroy(cf_shm_t* shm);
CF_DECLARE(cf_void_t*) cf_shm_get_buf(cf_shm_t* shm);
CF_DECLARE(cf_size_t)  cf_shm_get_size(cf_shm_t* shm);

CF_DECLS_END

/** @} */

#endif /* __CF_SHM_H__ */ 
