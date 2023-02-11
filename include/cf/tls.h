#ifndef __CF_THREAD_LOCAL_STORAGE_H__
#define __CF_THREAD_LOCAL_STORAGE_H__ 

#include <cf/types.h>
#if defined (CF_OS_WIN)
#include <Windows.h>
#else
#include <pthread.h>
#endif

/**
 * @defgroup CF_TLS TLS
 * Thread Local Storage
 * @{
 */

typedef struct cf_tls {
#if defined (CF_OS_WIN)
    DWORD tls_index;
#else
    pthread_key_t tls_key; 
#endif
} cf_tls_t;

CF_DECLS_BEGIN

typedef void(*cf_tls_memfree_f)(void* p);


cf_bool_t cf_tls_init(cf_tls_t* self, cf_tls_memfree_f f);
cf_bool_t cf_tls_deinit(cf_tls_t* self);
cf_bool_t cf_tls_set(cf_tls_t* self, void* value);
void* cf_tls_get(cf_tls_t* self);

CF_DECLS_END

/** @} */

#endif /* __CF_THREAD_LOCAL_STORAGE_H__ */
