#include <cf/dso.h>
#include <cf/err.h>
#include <cf/memory.h>

typedef struct cf_dso_handle {
    cf_void_t* handle;
} cf_dso_handle_t;

CF_DECLARE(cf_errno_t) cf_dso_load(cf_dso_handle_t** handle, const cf_char_t* path) {
    if(!handle || !path) return CF_EPARAM;

    *handle = (cf_dso_handle_t*)cf_malloc(sizeof(cf_dso_handle_t));
    if(!*handle) return CF_EMALLOC;


    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_dso_unload(cf_dso_handle_t* handle) {
    if(!handle) return CF_EPARAM;
    cf_free(handle);
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_dso_get_sym(cf_dso_handle_t* handle, const cf_char_t* name, cf_dso_sym_t* symbol) {
    if(!handle || !name || !symbol) return CF_EPARAM;

    *symbol = 0;
    return CF_OK;
}
