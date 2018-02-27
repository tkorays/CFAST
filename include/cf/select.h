#ifndef __CF_SELECT_H__
#define __CF_SELECT_H__

#include <cf/types.h>

#ifdef CF_OS_WIN

#else
#include <sys/select.h>
#endif

#define CF_FD_SET(n, p)     FD_SET(n, p)
#define CF_FD_CLR(n, p)     FD_CLR(n, p)
#define CF_FD_ISSET(n, p)   FD_ISSET(n, p)
#define CF_FD_ZERO(p)       FD_ZERO(p)
#define CF_FD_SETSIZE       FD_SETSIZE

typedef struct fd_set cf_fdset_t;

cf_errno_t cf_select(int maxfdp, cf_fdset_t* readfds, cf_fdset_t* writefds, cf_fdset_t* errorfds, 
    struct timeval* timeout);


#endif /* __CF_SELECT_H__ */
