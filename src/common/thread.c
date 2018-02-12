#include <cf/thread.h>
#include <cf/err.h>
#ifdef CF_OS_WIN
#include <process.h>
#else
#include <unistd.h>
#endif

cf_errno_t cf_thread_create(cf_thread_t* t, const cf_thread_attr_t* attr,
                            cf_void_t*(*call_func)(cf_void_t*), cf_void_t* arg) {
    if(pthread_create(t, attr, call_func, arg) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_thread_join(cf_thread_t t, void** arg) {
    if(pthread_join(t, arg) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_thread_detach(cf_thread_t t) {
    if(pthread_detach(t) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_thread_cancel(cf_thread_t t) {
    if(pthread_cancel(t) == 0) return CF_OK;
    else return CF_NOK;
}

cf_bool_t cf_thread_equal(cf_thread_t t1, cf_thread_t t2) {
    if(pthread_equal(t1, t2)) return CF_TRUE;
    else return CF_FALSE;
}

cf_thread_t cf_thread_self() {
    return pthread_self();
}

cf_errno_t cf_thread_attr_init(cf_thread_attr_t* attr) {
    if(!attr) return CF_EPARAM;
    if(pthread_attr_init(attr) == 0) return CF_OK;
    else return CF_NOK;
}

cf_errno_t cf_thread_attr_destroy(cf_thread_attr_t* attr) {
    if(!attr) return CF_EPARAM;
    if(pthread_attr_destroy(attr) == 0) return CF_OK;
    else return CF_NOK;
}

cf_uint_t cf_getpid() {
#ifdef CF_OS_WIN
    return(cf_uint_t)_getpid();
#else
    return (cf_uint_t)getpid();
#endif
}

cf_uint_t cf_gettid() {
#ifdef CF_OS_WIN
    return (cf_uint_t)pthread_self().p;
#else 
    return (cf_uint_t)pthread_self();
#endif
}
