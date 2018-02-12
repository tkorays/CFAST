#ifndef __CF_MUTEX_H__
#define __CF_MUTEX_H__

#include <cf/types.h>
#include <pthread.h>

typedef pthread_mutex_t cf_mutex_t;
typedef pthread_mutexattr_t cf_mutex_attr_t;

cf_errno_t cf_mutex_init(cf_mutex_t* mutex, const cf_mutex_attr_t* attr);
cf_errno_t cf_mutex_destroy(cf_mutex_t* mutex);
cf_errno_t cf_mutex_lock(cf_mutex_t* mutex);
cf_errno_t cf_mutex_trylock(cf_mutex_t* mutex);
cf_errno_t cf_mutex_unlock(cf_mutex_t* mutex);

#endif /* __CF_MUTEX_H__ */
