#include <cf/path.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/memory.h>

#ifdef CF_OS_WIN
    #include <windows.h>
    #include <io.h>
    #include <direct.h>
#else 
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

CF_DECLARE(cf_errno_t) cf_path_append(cf_char_t* path, cf_size_t size, const cf_char_t* p) {
    cf_size_t src_len, app_len;
    if(!path || size == 0 || !p) return CF_EPARAM;

    src_len = cf_strlen(path);
    app_len = cf_strlen(p);
    if(src_len + app_len > CF_PATH_MAX_SIZE) {
        return CF_ENOK;
    }

    if(src_len && (path[src_len - 1] != CF_PATH_SPLIT_CHAR)) {
        if(cf_strcat_s(path, size, CF_PATH_SPLIT_STR)) return CF_ENOK;
    }
    if(cf_strcat_s(path, size, p)) return CF_EOK;
    else return CF_ENOK;
}

CF_DECLARE(cf_errno_t) cf_path_getcwd(cf_char_t* path, cf_size_t size) {
    if(getcwd(path, size)) return CF_EOK;
    else return CF_ENOK;
}

CF_DECLARE(cf_errno_t) cf_path_realpath(const cf_char_t* org, cf_char_t* path, cf_size_t size) {
    if(!org || !path || !size) return CF_EPARAM;

    if(realpath(org, path)) return CF_EOK;
    
    return CF_ENOK;
}


CF_DECLARE(cf_errno_t) cf_path_basename(const cf_char_t* org, cf_char_t* path, cf_size_t size) {
    cf_int_t len, start, stop;
    if(!org || !path || !size) return CF_EPARAM;

    len = cf_strlen(org);
    if(!len) {
        *path = 0;
        return CF_EOK;;
    }

    start = -1;
    stop = len - 1; 
    while(--len >= 0) {
        if(org[len] == CF_PATH_SPLIT_CHAR_WIN || org[len] == CF_PATH_SPLIT_CHAR_UNI) {
            if(start >= 0) break;
            stop--;
        } else {
            start = len;
        }
    }

    if(stop - start + 2 > size) return CF_ENOK;

    while(start <= stop) *(path++) = org[start++];
    *path = 0;
    return CF_EOK;
}

CF_DECLARE(cf_errno_t) cf_path_dirname(const cf_char_t* org, cf_char_t* path, cf_size_t size) {
    cf_int_t len, start, stop;
    if(!org || !path || !size) return CF_EPARAM;
    len = cf_strlen(org);
    if(!len) {
        *path = 0;
        return CF_EOK;;
    }

    start = 0;
    stop = len - 1;

    while(org[stop] == CF_PATH_SPLIT_CHAR_WIN || org[stop] == CF_PATH_SPLIT_CHAR_UNI) stop--;

    if(start >= stop) {
        *path = 0;
        return CF_EOK;
    }

    while(start < stop 
        && org[stop] != CF_PATH_SPLIT_CHAR_WIN
        && org[stop] != CF_PATH_SPLIT_CHAR_UNI) stop--;
    
    while(start < stop) *(path++) = org[start++];
    *path = 0;
    
    return CF_EOK;
}

CF_DECLARE(cf_bool_t)  cf_path_exist(const cf_char_t* path) {
    if(!path) return CF_FALSE;
#ifdef CF_OS_WIN
    return _access(path, 0) == 0 ? CF_TRUE : CF_FALSE;
#else
    return access(path, 0) == 0 ? CF_TRUE : CF_FALSE;
#endif
}

CF_DECLARE(cf_bool_t)  cf_path_isfile(const cf_char_t* path) {
#ifdef CF_OS_WIN
    struct _stat st;
    if(!path) return CF_FALSE;
    if(_stat(path, &st) != 0) return CF_FALSE;
    return (st.st_mode & _S_IFMT) == _S_IFREG;
#else
    struct stat st;
    if(!path) return CF_FALSE;
    if(stat(path, &st) != 0) return CF_FALSE;
    return (st.st_mode & S_IFMT) == S_IFREG;
#endif
}

CF_DECLARE(cf_bool_t)  cf_path_isdir(const cf_char_t* path) {
#ifdef CF_OS_WIN
    struct _stat st;
    if(!path) return CF_FALSE;
    if(_stat(path, &st) != 0) return CF_FALSE;
    return (st.st_mode & _S_IFMT) == _S_IFDIR;
#else 
    struct stat st;
    if(!path) return CF_FALSE;
    if(stat(path, &st) != 0) return CF_FALSE;
    return (st.st_mode & S_IFMT) == S_IFDIR;
#endif
}
