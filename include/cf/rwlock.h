#ifndef __CFAST_RWLOCK_H__
#define __CFAST_RWLOCK_H__

#include "cf/types.h"
#ifdef CF_OS_WIN
#include <Windows.h>
#include <synchapi.h>
#else
#include <pthread.h>
#endif

struct cf_rwlock {
#ifdef CF_OS_WIN
    SRWLOCK             lock;
#else
    pthread_rwlock_t    lock;
#endif
};

CF_DECLS_BEGIN

/**
 * A read-write lock.
 */
typedef struct cf_rwlock cf_rwlock_t;


/**
 * Initializes a read-write lock.
 *
 * @param self The read-write lock to initialize.
 */
cf_void_t cf_rwlock_init(cf_rwlock_t* self);

/**
 * Deinitializes a read-write lock.
 *
 * @param self The read-write lock to deinitialize.
 */
cf_void_t cf_rwlock_deinit(cf_rwlock_t* self);

/**
 * Acquires a read lock on the read-write lock.
 *
 * @param self The read-write lock to acquire the read lock on.
 */
cf_void_t cf_rwlock_rdlock(cf_rwlock_t* self);

/**
 * Attempts to acquire a read lock on the read-write lock.
 *
 * @param self The read-write lock to attempt to acquire the read lock on.
 * @return CF_TRUE if the read lock was acquired, CF_FALSE otherwise.
 */
cf_bool_t cf_rwlock_try_rdlock(cf_rwlock_t* self);

/**
 * Acquires a write lock on the read-write lock.
 *
 * @param self The read-write lock to acquire the write lock on.
 */
cf_void_t cf_rwlock_wrlock(cf_rwlock_t* self);

/**
 * Attempts to acquire a write lock on the read-write lock.
 *
 * @param self The read-write lock to attempt to acquire the write lock on.
 * @return CF_TRUE if the write lock was acquired, CF_FALSE otherwise.
 */
cf_bool_t cf_rwlock_try_wrlock(cf_rwlock_t* self);

/**
 * Releases a read lock on the read-write lock.
 *
 * @param self The read-write lock to release the read lock on.
 */
cf_void_t cf_rwlock_rdunlock(cf_rwlock_t* self);

/**
 * Releases a write lock on the read-write lock.
 *
 * @param self The read-write lock to release the write lock on.
 */
cf_void_t cf_rwlock_wrunlock(cf_rwlock_t* self);

CF_DECLS_END

#endif /* __CFAST_RWLOCK_H__ */
