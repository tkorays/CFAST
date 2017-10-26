#ifndef __CF_TYPE_H__
#define __CF_TYPE_H__

#include <stdint.h>

#ifdef _WIN32
#define CF_OS_WIN 
#else 
#define CF_OS_LINUX 
#endif

#define CF_NULL (void*)0
#define CF_TRUE 1
#define CF_FALSE 0
#define cf_true 1
#define cf_false 0
#define CF_MIN(a, b) ((a) > (b) ? (b) : (a))
#define CF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CF_ARRAY_SIZE(arr, t) (sizeof(arr)/sizeof(t))

typedef uint32_t cf_id_t;
typedef void* cf_handle_t;
typedef uint32_t cf_size_t;
typedef uint8_t cf_bool_t;
typedef uintptr_t cf_ptr_t;
typedef enum {
    cf_ok = 0,
    cf_err,
    cf_param_err,
    cf_alloc_err,
} cf_ret_t;

typedef enum {
    cf_lan_en = 0,
    cf_lan_cn,
    cf_lan_other
} cf_lang_t;

#endif /* __CF_TYPE_H__ */
