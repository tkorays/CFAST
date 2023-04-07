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

/**
 * @brief Creates a shared memory object with the given size and filename.
 * 
 * @param shm Pointer to the shared memory object to be created.
 * @param size Size of the shared memory object to be created.
 * @param filename Name of the file to be associated with the shared memory object.
 * @return cf_errno_t Error code indicating the success or failure of the operation.
 */
CF_DECLARE(cf_errno_t) cf_shm_create(cf_shm_t** shm, cf_size_t size, const cf_char_t* filename);

/**
 * @brief Destroys the given shared memory object.
 * 
 * @param shm Pointer to the shared memory object to be destroyed.
 * @return cf_errno_t Error code indicating the success or failure of the operation.
 */
CF_DECLARE(cf_errno_t) cf_shm_destroy(cf_shm_t* shm);

/**
 * @brief Returns a pointer to the buffer associated with the given shared memory object.
 * 
 * @param shm Pointer to the shared memory object whose buffer is to be returned.
 * @return cf_void_t* Pointer to the buffer associated with the shared memory object.
 */
CF_DECLARE(cf_void_t*) cf_shm_get_buf(cf_shm_t* shm);

/**
 * @brief Returns the size of the buffer associated with the given shared memory object.
 * 
 * @param shm Pointer to the shared memory object whose buffer size is to be returned.
 * @return cf_size_t Size of the buffer associated with the shared memory object.
 */
CF_DECLARE(cf_size_t)  cf_shm_get_size(cf_shm_t* shm);



CF_DECLS_END

/** @} */

#endif /* __CF_SHM_H__ */ 
