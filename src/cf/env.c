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
    DWORD ret = GetEnvironmentVariable(name, value, size);
    if (ret == 0 || ret > size) {
        return CF_ENOK;
    }
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
    if (!SetEnvironmentVariable(name, value)) return CF_ENOK;
#else 
    if(0 > setenv(name, value, 1)) return CF_ENOK;
#endif

    return CF_EOK;
}

cf_errno_t cf_env_delete(const cf_char_t* name) {
    if(!name) return CF_EPARAM;
#ifdef CF_OS_WIN
    if (!SetEnvironmentVariable(name, NULL)) return CF_ENOK;
#else 
    unsetenv(name);
#endif
    return CF_EOK;
}

cf_errno_t cf_env_get_home(cf_char_t* path, cf_size_t size) {
    if(!path) return CF_EPARAM;
#ifdef CF_OS_WIN
    cf_errno_t ret = cf_env_get("HOMEDRIVE", path, size);
    if(ret != CF_EOK) return ret;
    ret = cf_env_get("HOMEPATH", path + strlen(path), size - strlen(path));
    if(ret != CF_EOK) return ret;
#else 
    cf_errno_t ret = cf_env_get("HOME", path, size);
    if(ret != CF_EOK) return ret;
#endif
    return CF_EOK;
}
