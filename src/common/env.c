#include <cf/env.h>
#include <cf/err.h>
#include <cf/str.h>

#ifdef CF_OS_WIN
    #include <windows.h>
#else 
    #include <unistd.h>
    #include <stdlib.h>
#endif

cf_errno_t cf_env_get(const cf_char_t* name, cf_char_t* value, cf_size_t size) {
    const char* v = 0;
    if(!name || !value) return CF_EPARAM;
#ifdef CF_OS_WIN

#else 
    v = getenv(name);
    if(!v) return CF_ENOK;

    if(!cf_strcpy_s(value, size, v)) return CF_ENOK;
#endif

    return CF_EOK;
}

cf_errno_t cf_env_set(const cf_char_t* name, const cf_char_t* value) {
    if(!name || !value) return CF_EPARAM;
#ifdef CF_OS_WIN

#else 
    if(0 > setenv(name, value, 1)) return CF_ENOK;
#endif

    return CF_EOK;
}

cf_errno_t cf_env_delete(const cf_char_t* name) {
    if(!name) return CF_EPARAM;
#ifdef CF_OS_WIN

#else 
    unsetenv(name);
#endif
    return CF_EOK;
}

cf_errno_t cf_env_get_home(cf_char_t* path, cf_size_t size) {
    return CF_EOK;
}
