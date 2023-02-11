/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    poll.h
 * @brief   Poll for linux/windows.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-3-4
 ****************************************************/
#ifndef __CF_POLL_H__
#define __CF_POLL_H__

#include <cf/types.h>
#ifdef CF_OS_WIN
#pragma error("Windows is NOT SUPPORTED!")
#else
#include <sys/poll.h>
#endif

/**
 * @defgroup CF_POLL poll
 * poll
 * @{
 */

CF_DECLS_BEGIN

#define CF_POLLIN       POLLIN
#define CF_POLLPRI      POLLPRI
#define CF_POLLOUT      POLLOUT
#define CF_POLLRDNORM   POLLRDNORM
#define CF_POLLWRNORM   POLLOUT
#define CF_POLLRDBNAD   POLLRDBAND 
#define CF_POLLWRBAND   POLLWRBAND

#define CF_POLLEXTEND   POLLEXTEND
#define CF_POLLATTRIB   POLLATTRIB
#define CF_POLLWRITE    POLLWRITE

#define CF_POLLERR      POLLERR     /* some poll error occurred */
#define CF_POLLHUP      POLLHUP     /* file descriptor was "hung up" */
#define CF_POLLNVAL     POLLNVAL    /* requested events "invalid" */

typedef struct pollfd cf_pollfd_t;
typedef nfds_t cf_nfds_t;

CF_DECLARE(cf_int_t) cf_poll(cf_pollfd_t* fdarray, cf_nfds_t nfds, cf_int_t timeout);

CF_DECLS_END

/** @} */

#endif /* __CF_POLL_H__ */
