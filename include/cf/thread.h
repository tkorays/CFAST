/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cli.h
 * @brief   thread wrapper.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-6
 ****************************************************/
#ifndef __CF_THREAD_H__
#define __CF_THREAD_H__

#define _GNU_SOURCE

#include <cf/types.h>
#ifdef CF_OS_WIN
#include <Windows.h>
#else
#include <pthread.h>
#endif

/**
 * @defgroup CF_THREAD thread
 * a system independent thread implementation
 * @{
 */

CF_DECLS_BEGIN

/**
 * @brief Thread priority.
 * 
 */
typedef enum {
    CF_THREAD_PRIORITY_LOW = 0,
    CF_THREAD_PRIORITY_NORMAL,
    CF_THREAD_PRIORITY_HIGH,
} cf_thread_priority_t;

#ifdef CF_OS_WIN

/** `CF_THREAD_DEF_PROC` is used to define the thread processing function. */
#define CF_THREAD_DEF_PROC(name, arg) cf_uint32_t WINAPI name(cf_void_t* arg)

/** `CF_THREAD_RET` is used to adapt for `CF_THREAD_DEF_PROC` return value */
#define CF_THREAD_RET(ret) ((cf_uint32_t)(ret))

/** native thread handle */
typedef HANDLE cf_thread_t;

/** native thread attribute */
typedef SECURITY_ATTRIBUTES cf_thread_attr_t;

/** prototype for thread processing */
typedef cf_uint32_t(WINAPI *cf_thread_proc_t)(cf_void_t*);
#else

/** `CF_THREAD_DEF_PROC` is used to define the thread processing function. */
#define CF_THREAD_DEF_PROC(name, arg) cf_void_t* name(cf_void_t* arg)

/** `CF_THREAD_RET` is used to adapt for `CF_THREAD_DEF_PROC` return value */
#define CF_THREAD_RET(ret) ((cf_void_t*)(ret))

/** native thread handle */
typedef pthread_t cf_thread_t;

/** native thread attribute */
typedef pthread_attr_t cf_thread_attr_t;

/** prototype for thread processing */
typedef cf_void_t*(*cf_thread_proc_t)(cf_void_t*);
#endif

/**
 * @brief create a thread with the thread attribute and processing function.
 * 
 * @param t     the output thread handle
 * @param attr  thread attribute
 * @param proc  processing function
 * @param arg   passing arg for processing function.
 * @return cf_errno_t success or failed errno
 */
cf_errno_t cf_thread_create(cf_thread_t* t, cf_thread_attr_t* attr,
    cf_thread_proc_t proc, cf_void_t* arg);

/**
 * @brief exit the current thread.
 * 
 * @param code  exit with the code
 * @return cf_void_t 
 */
cf_void_t cf_thread_exit(cf_uint32_t code);

/**
 * @brief join a thread
 * 
 * @param t         thread handle
 * @param retval    return value for the joined thread
 * @return cf_errno_t 
 */
cf_errno_t cf_thread_join(cf_thread_t t, cf_uint32_t* retval);

/**
 * @brief detach a thread
 * 
 * @param t         thread handle
 * @return cf_errno_t 
 */
cf_errno_t cf_thread_detach(cf_thread_t t);

/**
 * @brief cancle a thread
 * 
 * @param t         thread handle
 * @return cf_errno_t 
 */
cf_errno_t cf_thread_cancel(cf_thread_t t);

/**
 * @brief is the same thread ?
 * 
 * @param t1        compared thread
 * @param t2        other thread
 * @return cf_bool_t 
 */
cf_bool_t cf_thread_equal(cf_thread_t t1, cf_thread_t t2);

/**
 * @brief set thread name
 * 
 * @param t         thread handle
 * @param name      thread name
 */
void cf_thread_setname(cf_thread_t t, const char* name);

/**
 * @brief set thread priority.
 * 
 * @param t         thread handle
 * @param p         priority of the thread
 */
void cf_thread_set_priority(cf_thread_t t, cf_thread_priority_t p);

/**
 * @brief return the current thread handle.
 * 
 * @return cf_thread_t 
 */
cf_thread_t cf_thread_self();

/**
 * @brief create a thread attribute
 * 
 * @param attr      output thread attribute
 * @return cf_errno_t 
 */
cf_errno_t cf_thread_attr_init(cf_thread_attr_t* attr);

/**
 * @brief destroy a thread attribute.
 * 
 * @param attr      thread attribute that will be destroyed.
 * @return cf_errno_t 
 */
cf_errno_t cf_thread_attr_destroy(cf_thread_attr_t* attr);

/**
 * @brief get process id.
 * 
 * @return cf_uint_t process id
 */
cf_uint_t cf_getpid();

/**
 * @brief get thread id.
 * 
 * @return cf_uint_t thread id.
 */
cf_uint_t cf_gettid();

CF_DECLS_END

/** @} */

#endif /* __CF_THREAD_H__ */
