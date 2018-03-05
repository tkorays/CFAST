#include <cf/poll.h>

CF_DECLARE(cf_int_t) cf_poll(cf_pollfd_t* fdarray, cf_nfds_t nfds, cf_int_t timeout) {
    return poll(fdarray, nfds, timeout);
}
