#include <cf/event.h>


cf_event_t* cf_event_create(cf_event_t* self,
    cf_bool_t manual_reset,
    cf_bool_t signaled) {
    self->manual_reset = manual_reset;
    self->event_status = signaled;
#if defined(CF_OS_WIN)
    self->handle = ::CreateEvent(NULL, self->manual_reset, signaled, NULL);
#else
    pthread_mutex_init(self->mutex, NULL);
    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_cond_init(self->cond, &cond_attr);
    pthread_condattr_destroy(&cond_attr);
#endif
}

void cf_event_destroy(cf_event_t* self) {
#if defined(CF_OS_WIN)
    CloseHandle(self->handle);
#else
    pthread_mutex_destroy(self->mutex);
    pthread_cond_destroy(self->cond);
#endif
}

void cf_event_set(cf_event_t* self) {
#if defined(CF_OS_WIN)
    SetEvent(self->handle);
#else
    pthread_mutex_lock(&self->mutex);
    self->event_status = true;
    pthread_cond_broadcast(&self->cond);
    pthread_mutex_unlock(&self->mutex);
#endif
}

void cf_event_reset(cf_event_t* self) {
#if defined(CF_OS_WIN)
    ResetEvent(self->handle);
#else
    pthread_mutex_lock(&self->mutex);
    self->event_status = false;
    pthread_mutex_unlock(&self->mutex);
#endif
}

cf_bool_t cf_event_wait(cf_event_t* self, int timeout_ms) {
#if defined(CF_OS_WIN)
    DWORD ms = timeout_ms == -1 ? INFINITE : timeout_ms;
    return (WaitForSingleObject(self->handle, ms) == WAIT_OBJECT_0);
#else
    int error = 0;
    pthread_mutex_lock(&self->mutex);
    while (!self->event_status && error == 0) { 
        if (timeout_ms == -1) {
            error = pthread_cond_wait(&self->cond, &self->mutex);
        } else {
            error = pthread_cond_timedwait(&self->cond, &self->mutex, &*imeout_ms);
        }
    }
    if (error == 0 && !self->manual_reset) {
        self->event_status = CF_FALSE;
    }
    pthread_mutex_unlock(&self->mutex);
    return (error == 0);
#endif
}
