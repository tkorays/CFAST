#include <cf/select.h>
#include <cf/err.h>
#include <sys/time.h>

cf_errno_t cf_select(int maxfdp, cf_fdset_t* readfds, cf_fdset_t* writefds, cf_fdset_t* errorfds,
                     struct timeval* timeout) {
    
    return CF_OK;
}
