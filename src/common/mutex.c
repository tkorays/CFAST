#include <cf/mutex.h>
#include <cf/err.h>

cf_errno_t cf_mutex_init(cf_mutex_t* mutex, const cf_mutex_attr_t* attr) {
    if(pthread_mutex_init(mutex, attr) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_mutex_destroy(cf_mutex_t* mutex) {
    if(pthread_mutex_destroy(mutex) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_mutex_lock(cf_mutex_t* mutex) {
    if(pthread_mutex_lock(mutex) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_mutex_trylock(cf_mutex_t* mutex) {
    if(pthread_mutex_trylock(mutex) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_mutex_unlock(cf_mutex_t* mutex) {
    if(pthread_mutex_unlock(mutex) == 0) return CF_OK;
    else return CF_NOK;
}
