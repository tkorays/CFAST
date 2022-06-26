#ifndef __CF_EVENT_H__
#define __CF_EVENT_H__

#include <cf/types.h>

#if defined(CF_OS_WIN)
#include <windows.h>
#else
#include <pthread.h>
#endif

CF_DECLS_BEGIN

typedef struct cf_event {
    cf_bool_t managed;
#if defined(CF_OS_WIN)
    HANDLE handle;
#else
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    cf_bool_t manual_reset;
    cf_bool_t event_status;
#endif
} cf_event_t;

cf_event_t* cf_event_create(cf_event_t* self,
    cf_bool_t manual_reset,
    cf_bool_t signaled);
void cf_event_destroy(cf_event_t* self);
void cf_event_set(cf_event_t* self);
void cf_event_reset(cf_event_t* self);
cf_bool_t cf_event_wait(cf_event_t* self, int timeout_ms);

CF_DECLS_END

#endif /* __CF_EVENT_H__ */

