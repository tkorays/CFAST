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

CF_DECLS_BEGIN
#ifdef CF_OS_WIN
typedef HANDLE cf_mutex_t;
typedef SECURITY_ATTRIBUTES cf_mutex_attr_t;
#else
typedef pthread_mutex_t cf_mutex_t;
typedef pthread_mutexattr_t cf_mutex_attr_t;
#endif

cf_errno_t cf_mutex_init(cf_mutex_t* mutex, const cf_mutex_attr_t* attr);
cf_errno_t cf_mutex_destroy(cf_mutex_t* mutex);
cf_errno_t cf_mutex_lock(cf_mutex_t* mutex);
cf_errno_t cf_mutex_trylock(cf_mutex_t* mutex);
cf_errno_t cf_mutex_unlock(cf_mutex_t* mutex);

CF_DECLS_END

#endif /* __CF_MUTEX_H__ */
