#include <cf/mutex.h>
#include <cf/err.h>

cf_errno_t cf_mutex_init(cf_mutex_t* mutex, const cf_mutex_attr_t* attr) {
#ifdef CF_OS_WIN
    if (!mutex) return CF_EPARAM;
    *mutex = CreateMutex(NULL, TRUE, NULL);
    if (*mutex == NULL) return CF_NOK;
    else return CF_OK;
#else
    if(pthread_mutex_init(mutex, attr) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_mutex_destroy(cf_mutex_t* mutex) {
#ifdef CF_OS_WIN
    if (!mutex) return CF_EPARAM;   
    return CloseHandle(*mutex) ? CF_OK : CF_NOK;
#else
    if(pthread_mutex_destroy(mutex) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_mutex_lock(cf_mutex_t* mutex) {
#ifdef CF_OS_WIN
    if (!mutex) return CF_EPARAM;

    // WAIT_TIMEOUT 
    if (WAIT_OBJECT_0 == WaitForSingleObject(*mutex, INFINITE)) return CF_OK;
    else return CF_NOK;
#else
    if(pthread_mutex_lock(mutex) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_mutex_trylock(cf_mutex_t* mutex) {
#ifdef CF_OS_WIN
    if (!mutex) return CF_EPARAM;

    if (WAIT_OBJECT_0 == WaitForSingleObject(*mutex, 0)) return CF_OK;
    else return CF_NOK;
#else
    if(pthread_mutex_trylock(mutex) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_mutex_unlock(cf_mutex_t* mutex) {
#ifdef CF_OS_WIN
    if (!mutex) return CF_EPARAM;
    return ReleaseMutex(*mutex) ? CF_OK : CF_NOK;
#else
    if(pthread_mutex_unlock(mutex) == 0) return CF_OK;
    else return CF_NOK;
#endif
}
