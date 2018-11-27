#include <cf/path.h>
#include <cf/err.h>
#include <cf/str.h>
#include <cf/memory.h>


CF_DECLARE(cf_errno_t) cf_path_init(cf_path_t* path, const cf_char_t* p) {
    cf_size_t len = 0;
    if(!path) return CF_EPARAM;
    if(p) {
        len = cf_strlen(p);
        if(len) {
            cf_strncpy_s(path->path, sizeof(CF_PATH_MAX_SIZE), p, len);
            path->len = len;
            return CF_EOK;
        }
    } 

    path->path[0] = '\0';
    path->len = 0;
    
}

CF_DECLARE(cf_errno_t) cf_path_empty(cf_path_t* path) {
    if(!path) return CF_EPARAM;
    path->path[0] = '\0';
    path->len = 0;
    return CF_EOK;
}

CF_DECLARE(cf_errno_t) cf_path_append(cf_path_t* path, const cf_char_t* p) {
    if(!path || !p) return CF_EPARAM;
    
}

CF_DECLARE(cf_errno_t) cf_path_rel(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_abs(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_basename(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_dirname(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_bool_t)  cf_path_exist(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isabs(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isfile(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isdir(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_walk(const cf_path_t* path, cf_bool_t(*walk_func)(const cf_path_t*));