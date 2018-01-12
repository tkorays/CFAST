#include "cfast/cf_thread_if.h"
#include "pthread.h"
#ifdef CF_OS_WIN
#include <process.h>
#else
#include <unistd.h>
#endif

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
