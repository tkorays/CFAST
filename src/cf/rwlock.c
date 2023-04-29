#include "cf/rwlock.h"
#include "cf/memory.h"

/**
 * for windows, see
 * https://learn.microsoft.com/en-us/windows/win32/sync/synchronization-functions
 */

cf_void_t cf_rwlock_init(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    InitializeSRWLock(&self->lock);
#else
    pthread_rwlock_init(&self->lock, NULL);
#endif
}

cf_void_t cf_rwlock_deinit(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
#else
    if (self) {
        pthread_rwlock_destroy(&self->lock);
    }
#endif
}

cf_void_t cf_rwlock_rdlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    AcquireSRWLockShared(&self->lock);
#else
    pthread_rwlock_rdlock(&self->lock);
#endif
}

cf_bool_t cf_rwlock_try_rdlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    if (TryAcquireSRWLockShared(&self->lock) == 0) {
        return CF_FALSE;
    }
#else
    if (pthread_rwlock_tryrdlock(&self->lock) != 0) {
        return CF_FALSE;
    }
#endif
    return CF_TRUE;
}

cf_void_t cf_rwlock_wrlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    AcquireSRWLockExclusive(&self->lock);
#else
    pthread_rwlock_wrlock(&self->lock);
#endif
}

cf_bool_t cf_rwlock_try_wrlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    if (TryAcquireSRWLockExclusive(&self->lock) == 0) {
        return CF_FALSE;
    }
#else
    if (pthread_rwlock_trywrlock(&self->lock) != 0) {
        return CF_FALSE;
    }
#endif
    return CF_TRUE;
}

cf_void_t cf_rwlock_rdunlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    ReleaseSRWLockShared(&self->lock);
#else
    pthread_rwlock_unlock(&self->lock);
#endif
}

cf_void_t cf_rwlock_wrunlock(cf_rwlock_t* self) {
#ifdef CF_OS_WIN
    ReleaseSRWLockExclusive(&self->lock);
#else
    pthread_rwlock_unlock(&self->lock);
#endif
}

