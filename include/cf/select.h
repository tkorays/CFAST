/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    select.h
 * @brief   Select for linux/windows.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-2-28
 ****************************************************/
#ifndef __CF_SELECT_H__
#define __CF_SELECT_H__

#include <cf/types.h>
#include <cf/time.h>

#ifdef CF_OS_WIN

#else
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

/**
 * @defgroup CF_SELECT select
 * select
 * @{
 */

CF_DECLS_BEGIN

#ifndef CF_OS_WIN

/**
 * FD_SETSIZE
 */
extern const cf_size_t CF_FD_SETSIZE;

/**
 * fd_set
 */
typedef /*struct*/ fd_set cf_fdset_t;

typedef struct timeval timeval;

/**
 * @brief Sets all file descriptors in the set to zero.
 * 
 * @param fdset The set of file descriptors to be zeroed.
 */
CF_DECLARE(cf_void_t) cf_fd_zero(cf_fdset_t* fdset);

/**
 * @brief Adds a file descriptor to the set.
 * 
 * @param fd The file descriptor to be added.
 * @param fdset The set of file descriptors to which the file descriptor is to be added.
 */
CF_DECLARE(cf_void_t) cf_fd_set(cf_int_t fd, cf_fdset_t* fdset);

/**
 * @brief Clears a file descriptor from the set.
 * 
 * @param fd The file descriptor to be cleared.
 * @param fdset The set of file descriptors from which the file descriptor is to be cleared.
 */
CF_DECLARE(cf_void_t) cf_fd_clr(cf_int_t fd, cf_fdset_t* fdset);

/**
 * @brief Checks if a file descriptor is set in the set.
 * 
 * @param fd The file descriptor to be checked.
 * @param fdset The set of file descriptors to be checked.
 * @return cf_bool_t Returns true if the file descriptor is set in the set, false otherwise.
 */
CF_DECLARE(cf_bool_t) cf_fd_isset(cf_int_t fd, cf_fdset_t* fdset);


/**
 * @brief Waits for one of a set of file descriptors to become ready to perform I/O operations.
 * 
 * @param maxfdp The maximum file descriptor number to be checked.
 * @param rfds The set of file descriptors to be checked for read operations.
 * @param wfds The set of file descriptors to be checked for write operations.
 * @param efds The set of file descriptors to be checked for error conditions.
 * @param timeout The maximum time to wait for an I/O operation to become ready.
 * @return cf_errno_t Returns the number of ready file descriptors, or -1 if an error occurred.
 */
CF_DECLARE(cf_errno_t) cf_select(int maxfdp, cf_fdset_t* rfds, cf_fdset_t* wfds, cf_fdset_t* efds, struct timeval* timeout);


#endif

CF_DECLS_END

/** @} */

#endif /* __CF_SELECT_H__ */
