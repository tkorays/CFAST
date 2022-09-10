#include "cf/tls.h"

cf_bool_t cf_tls_init(cf_tls_t* self, cf_tls_memfree_f f) {
#if defined (CF_OS_WIN)
    self->tls_index = -1;
    self->tls_index = TLSAlloc();
#else
    self->tls_key = -1;
    if (0 != pthread_key_create(&self->tls_key, f)) {
        return CF_FALSE;
    }
#endif
    return CF_TRUE;
}

cf_bool_t cf_tls_deinit(cf_tls_t* self) {
#if defined (CF_OS_WIN)
    BOOL ret = TLSFree(self->tls_index);
    self->tls_index = -1;
    return CF_TYPE_CAST(cf_bool_t, ret);
#else
    if (0 != pthread_key_delete(self->tls_key)) {
        return CF_FALSE;
    }
    self->tls_key = CF_NULL_PTR;
#endif
    return CF_TRUE;
}

cf_bool_t cf_tls_set(cf_tls_t* self, void* value) {
#if defined (CF_OS_WIN)
    return CF_TYPE_CAST(cf_bool_t, TLSSetValue(self->tls_index, value));
#else
    return (0 != pthread_setspecific(self->tls_key, value));
#endif
}

void* cf_tls_get(cf_tls_t* self) {
#if defined (CF_OS_WIN)
    return TLSGetValue(self->tls_index);
#else
    return pthread_getspecific(self->tls_key);
#endif
}