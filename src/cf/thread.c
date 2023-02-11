#include <cf/thread.h>
#include <cf/err.h>
#ifdef CF_OS_WIN
#include <process.h>
#else
#include <unistd.h>
#endif

cf_errno_t cf_thread_create(cf_thread_t* t, cf_thread_attr_t* attr,
                            cf_thread_proc_t proc, cf_void_t* arg) {
#ifdef CF_OS_WIN
    cf_uint32_t tid = 0;
    *t = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)proc, arg, 0, &tid);
    if (*t == NULL) return CF_NOK;
    else return CF_OK;
#else
    if(pthread_create(t, attr, proc, arg) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_void_t cf_thread_exit(cf_uint32_t code) {
#ifdef CF_OS_WIN
    ExitThread(code);
#else
    /* 为了和windows保持一致 */
    pthread_exit((cf_void_t*)(cf_uintptr_t)code);
#endif
}

cf_errno_t cf_thread_join(cf_thread_t t, cf_uint32_t* retval) {
#ifdef CF_OS_WIN
    cf_uint32_t ret = WaitForSingleObject(t, INFINITE);
    if(retval) (cf_void_t)GetExitCodeThread(t, retval);
    if (WAIT_OBJECT_0 == ret) return CF_OK;
    else return CF_NOK;
#else
    cf_void_t* ret = 0;
    if(pthread_join(t, &ret) == 0) {
        if(retval) *retval = (cf_uint32_t)(cf_uintptr_t)ret;
        return CF_OK;
    }
    else return CF_NOK;
#endif
}

cf_errno_t cf_thread_detach(cf_thread_t t) {
#ifdef CF_OS_WIN
    return CloseHandle(t) ? CF_OK : CF_NOK;
#else
    if(pthread_detach(t) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_thread_cancel(cf_thread_t t) {
#ifdef CF_OS_WIN
    return TerminateThread(t, 0) ? CF_OK : CF_NOK;
#elif defined CF_OS_ANDROID
    /* NOT SUPPORTED! */
    return CF_NOK;
#else
    if(pthread_cancel(t) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_bool_t cf_thread_equal(cf_thread_t t1, cf_thread_t t2) {
#ifdef CF_OS_WIN
    return (t1 == t2) ? CF_OK : CF_NOK;
#else
    if(pthread_equal(t1, t2)) return CF_TRUE;
    else return CF_FALSE;
#endif
}

cf_thread_t cf_thread_self() {
#ifdef CF_OS_WIN
    return (cf_thread_t)GetCurrentThread();
#else
    return pthread_self();
#endif
}

cf_errno_t cf_thread_attr_init(cf_thread_attr_t* attr) {
#ifdef CF_OS_WIN
    return CF_OK;
#else
    if(!attr) return CF_EPARAM;
    if(pthread_attr_init(attr) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_errno_t cf_thread_attr_destroy(cf_thread_attr_t* attr) {
#ifdef CF_OS_WIN
    return CF_OK;
#else
    if(!attr) return CF_EPARAM;
    if(pthread_attr_destroy(attr) == 0) return CF_OK;
    else return CF_NOK;
#endif
}

cf_uint_t cf_getpid() {
#ifdef CF_OS_WIN
    return(cf_uint_t)GetCurrentProcessId();
#else
    return (cf_uint_t)getpid();
#endif
}

cf_uint_t cf_gettid() {
#ifdef CF_OS_WIN
    return GetCurrentThreadId();
#else 
    return (cf_uint_t)(cf_uintptr_t)pthread_self();
#endif
}
