#include <cf/select.h>
#include <cf/err.h>
#include <sys/time.h>


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

CF_DECLARE(cf_bool_t) cf_fd_isset(cf_int_t fd, cf_fdset_t* fdset) {
    return FD_ISSET(fd, fdset);
}

CF_DECLARE(cf_errno_t) cf_select(int maxfdp, cf_fdset_t* rfds, cf_fdset_t* wfds, cf_fdset_t* efds, cf_timeval_t* timeout) {
    struct timeval t;
    cf_int_t ret;
    t.tv_sec = timeout->tv_sec;
    t.tv_usec = timeout->tv_usec;
    
    ret = select(maxfdp, rfds, wfds, efds, &t);
    if(ret == 0) return CF_ESELECT_TOUT;
    else if(ret < 0) return CF_NOK;
    return CF_OK;
}
