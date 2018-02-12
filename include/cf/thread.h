#ifndef __CF_THREAD_H__
#define __CF_THREAD_H__

#include <cf/types.h>
#include <pthread.h>

typedef pthread_t cf_thread_t;
typedef pthread_attr_t cf_thread_attr_t;

cf_errno_t cf_thread_create(cf_thread_t* t, const cf_thread_attr_t* attr,
    cf_void_t*(*call_func)(cf_void_t*), cf_void_t* arg);
cf_errno_t cf_thread_join(cf_thread_t t, void** arg);
cf_errno_t cf_thread_detach(cf_thread_t t);
cf_errno_t cf_thread_cancel(cf_thread_t t);
cf_bool_t cf_thread_equal(cf_thread_t t1, cf_thread_t t2);
cf_thread_t cf_thread_self();
cf_errno_t cf_thread_attr_init(cf_thread_attr_t* attr);
cf_errno_t cf_thread_attr_destroy(cf_thread_attr_t* attr);

cf_uint_t cf_getpid();
cf_uint_t cf_gettid();


#endif /* __CF_THREAD_H__ */
