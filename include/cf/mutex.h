/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    mutex.h
 * @brief   Configuration for CFAST.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-2-10
 ****************************************************/
#ifndef __CF_MUTEX_H__
#define __CF_MUTEX_H__

#include <cf/types.h>

#ifdef CF_OS_WIN
#include <Windows.h>
#else
#include <pthread.h>
#endif

/**
 * defgroup CF_MUTEX mutex
 * a system independent mutex
 * @{
 */

CF_DECLS_BEGIN
#ifdef CF_OS_WIN
typedef HANDLE cf_mutex_t; /**< Windows mutex type */
typedef SECURITY_ATTRIBUTES cf_mutex_attr_t; /**< Windows mutex attribute type */
#else
typedef pthread_mutex_t cf_mutex_t; /**< POSIX mutex type */
typedef pthread_mutexattr_t cf_mutex_attr_t; /**< POSIX mutex attribute type */
#endif

/**
 * @defgroup CF_MUTEX Mutex
 * @brief A system independent mutex
 * @{
 */

/**
 * @brief Initialize a mutex
 * @param mutex Pointer to the mutex to initialize
 * @param attr Pointer to the mutex attribute to use
 * @return CF_EOK on success, otherwise an error code
 */
cf_errno_t cf_mutex_init(cf_mutex_t* mutex, const cf_mutex_attr_t* attr);

/**
 * @brief Destroy a mutex
 * @param mutex Pointer to the mutex to destroy
 * @return CF_EOK on success, otherwise an error code
 */
cf_errno_t cf_mutex_destroy(cf_mutex_t* mutex);

/**
 * @brief Lock a mutex
 * @param mutex Pointer to the mutex to lock
 * @return CF_EOK on success, otherwise an error code
 */
cf_errno_t cf_mutex_lock(cf_mutex_t* mutex);

/**
 * @brief Try to lock a mutex
 * @param mutex Pointer to the mutex to try to lock
 * @return CF_EOK on success, otherwise an error code
 */
cf_errno_t cf_mutex_trylock(cf_mutex_t* mutex);

/**
 * @brief Unlock a mutex
 * @param mutex Pointer to the mutex to unlock
 * @return CF_EOK on success, otherwise an error code
 */
cf_errno_t cf_mutex_unlock(cf_mutex_t* mutex);

/** @} */

CF_DECLS_END


/** @} */

#endif /* __CF_MUTEX_H__ */
