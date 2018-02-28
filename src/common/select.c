#include <cf/select.h>
#include <cf/err.h>
#include <sys/time.h>

#ifdef CF_OS_WIN

#else
#include <sys/select.h>
#endif


const cf_size_t CF_FD_SETSIZE = FD_SETSIZE;

CF_DECLARE(cf_void_t) cf_fd_zero(cf_fdset_t* fdset) {
    FD_ZERO(fdset);
}

CF_DECLARE(cf_void_t) cf_fd_set(cf_int_t fd, cf_fdset_t* fdset) {
    FD_SET(fd, fdset);
}

CF_DECLARE(cf_void_t) cf_fd_clr(cf_int_t fd, cf_fdset_t* fdset) {
    FD_CLR(fd, fdset);
}

CF_DECLARE(cf_void_t) cf_fd_isset(cf_int_t fd, cf_fdset_t* fdset) {
    FD_ISSET(fd, fdset);
}

CF_DECLARE(cf_errno_t) cf_select(int maxfdp, cf_fdset_t* readfds, cf_fdset_t* writefds, cf_fdset_t* errorfds,
                     struct timeval* timeout) {
    if(!readfds || !writefds || !errorfds || !timeout) return CF_EPARAM;
    return select(maxfdp, readfds, writefds, errorfds, timeout) > 0 ? CF_OK : CF_NOK;
}
