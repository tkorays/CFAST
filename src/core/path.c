#include <cf/path.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/memory.h>

#ifdef CF_OS_WIN
    #include <windows.h>
    #include <io.h>
    #include <direct.h>
    #include <sys/stat.h>
#else
    #include <unistd.h>
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
        if(!cf_strcat_s(path, size, CF_PATH_SPLIT_STR)) return CF_ENOK;
    }
    if(cf_strcat_s(path, size, p)) return CF_EOK;
    else return CF_ENOK;
}

CF_DECLARE(cf_errno_t) cf_path_getcwd(cf_char_t* path, cf_size_t size) {
#ifdef CF_OS_WIN
    if (_getcwd(path, size)) return CF_EOK;
#else 
    if(getcwd(path, size)) return CF_EOK;
#endif
    else return CF_ENOK;
}

CF_DECLARE(cf_errno_t) cf_path_realpath(const cf_char_t* org, cf_char_t* out, cf_size_t size) {
    cf_char_t tmps[CF_PATH_MAX_SIZE];
    cf_char_t *pin = tmps, *pout;
    cf_uint8_t state = 0;
    cf_errno_t e = CF_EOK;
    if(!org || !out || !size) return CF_EPARAM;

    if(!cf_path_isabs(org)) {
        e = cf_path_getcwd(tmps, CF_PATH_MAX_SIZE);
        if(e != CF_EOK) return CF_ENOK;

        e = cf_path_append(tmps, CF_PATH_MAX_SIZE, org);
        if(e != CF_EOK) return CF_ENOK;
    } else {
        cf_strcpy_s(tmps, CF_PATH_MAX_SIZE, org);
    }

    // a/../a.//./././ /a  /. //
    pout = out;
    /* scan pin and output to pout */
    while(*pin && (pin - tmps < CF_PATH_MAX_SIZE)) {
        //printf("get char: %c\n", *pin);
        /* initial state */
        if(state == 0) {
            if(*pin == CF_PATH_SPLIT_CHAR) {
                state = 1;
            } 
            *(pout++) = *pin;
        } 
        /* state 1: / */
        else if(state == 1) {
            if(*pin == CF_PATH_SPLIT_CHAR) ; /* double slash, skip */
            else if(*pin == '.') state = 2; /* state: /. */
            else { 
                state = 0;
                *(pout++) = *pin;
            }
        } 
        /* state 2: /. */
        else if(state == 2) {
            if(*pin == CF_PATH_SPLIT_CHAR) state = 1; /* /./ ----> / */
            else if(*pin == '.') state = 3; /* /.. */
            else { 
                state = 0;
                *(pout++) = '.';
                *(pout++) = *pin;
            }
        } 
        /* state 3: /.. */
        else if(state == 3) {
            /* /a/b/c/../ -> /a/b/ */
            if(*pin == CF_PATH_SPLIT_CHAR) {
                for(pout -= 2; pout >= out && *pout != CF_PATH_SPLIT_CHAR; pout--) ;
                if(pout < out) return CF_ENOK;
                pout++;
                state = 1;
            }
            /* /... or /..abc */
            else if(*pin == '.') { 
                state = 0; 
                *(pout++) = '.';
                *(pout++) = '.';
                *(pout++) = *pin;
            }
        }
        pin++;
    }
    *pout = '\0';

    return CF_EOK;
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

    if((cf_size_t)(stop - start + 2) > size) return CF_ENOK;

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

CF_DECLARE(cf_bool_t)  cf_path_isabs(const cf_char_t* path) {
#ifdef CF_OS_WIN
    if(path && (*path >= 'a' && *path <='z') || (*path >= 'A' && *path <= 'Z') && path[1] == ':') 
        return CF_TRUE;
    else return CF_FALSE;

#else 
    if(path && path[0] == CF_PATH_SPLIT_CHAR_UNI) return CF_TRUE;
    else return CF_FALSE;
#endif
}

