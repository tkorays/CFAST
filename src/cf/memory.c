#include "cf/types.h"
#include <cf/memory.h>
#include <stdlib.h>
#include <string.h>
#include "cf/hashtbl.h"
#include "cf/mutex.h"
#include "cf/assert.h"
#include "cf/location.h"
#include "cf/str.h"

#ifdef _WIN32
#pragma comment(lib, "DbgHelp.lib")
#include <DbgHelp.h>
HANDLE g_current_process = 0;
#endif

#ifdef CF_MEMORY_DBG
#include <stdio.h>

typedef struct {
    cf_location_t   location;
    cf_size_t       size;
    cf_uint8_t      info[256];
} malloc_info;

struct cf_memchk_ctx {
    cf_bool_t       inited;

    /** address/size pair, we use the lower 32bits as hash and size as value */
    cf_hashtbl_t*   alloc_info;
    cf_mutex_t      mutex;
    cf_size_t       alloc_size;
} g_memchk_ctx;

void CF_MEMTRACE_MALLOC(void* p, cf_size_t size,
                        const cf_char_t* file,
                        const cf_char_t* function,
                        int line) {
    malloc_info* info;
    cf_size_t len;
    if (!g_memchk_ctx.inited) return;

    /* printf("ALLOC p: %p, size: %ld, file: %s, func: %s, line: %d\n", p, size, file, function, line); */

    cf_mutex_lock(&g_memchk_ctx.mutex);

    g_memchk_ctx.alloc_size += size;

    /**
     * fill the malloc info and add to hashtbl
     * use the allocated address as the key, eg:
     * void* p = malloc(size);
     * p is 0x12345678, then key is:
     * char key[] = {0x12, 0x34, 0x56, 0x78};
     * int keylen = sizeof(void*);
     */
    {
        info = cf_malloc_z_native(sizeof(malloc_info));

        len = cf_strlen(file);
        info->location.file_name = cf_malloc_z_native(len + 1);
        cf_strcpy_s((char*)info->location.file_name, len + 1, file);

        len = cf_strlen(function);
        info->location.function_name = cf_malloc_z_native(cf_strlen(function) + 1);
        cf_strcpy_s((char*)info->location.function_name, len + 1, function);

        info->location.line_number = line;
        info->size = size;


#ifdef _WIN32
        if (g_current_process == 0) {
            g_current_process = GetCurrentProcess();
            SymInitialize(g_current_process, NULL, TRUE);
        }
        void* stack[10];
        DWORD frames = CaptureStackBackTrace(2, 5, stack, NULL);
        DWORD i;
        for (i = 0; i < frames; i++) {
            DWORD64 addr = (DWORD64)stack[i];
            PSYMBOL_INFO sym_info = cf_malloc_native(sizeof(SYMBOL_INFO) + 64);
            sym_info->SizeOfStruct = sizeof(SYMBOL_INFO);
            sym_info->MaxNameLen = 64;
            DWORD64 displacement_sym = 0;
            if (SymFromAddr(g_current_process, addr, &displacement_sym, sym_info)) {
                cf_snprintf(info->info, sizeof(info->info) - cf_strlen(info->info),
                    "%s<-%s", info->info, sym_info->Name);
                    //SymGetLineFromAddr()
            }
            cf_free_native(sym_info);
        }
#endif
    }
    cf_hashtbl_set(g_memchk_ctx.alloc_info, &p, sizeof(void*), (cf_void_t*)info);

    cf_mutex_unlock(&g_memchk_ctx.mutex);
}

void CF_MEMTRACE_FREE(void* p,
                      const cf_char_t* file,
                      const cf_char_t* function,
                      int line) {
    malloc_info* info;
    if (!g_memchk_ctx.inited) return;

    /* printf("free: %p, file: %s, func: %s, line: %d\n", p, file, function, line); */
    cf_mutex_lock(&g_memchk_ctx.mutex);

    info = CF_TYPE_CAST(malloc_info*, cf_hashtbl_get(g_memchk_ctx.alloc_info, &p, sizeof(void*)));
    if (info) {
        cf_assert(g_memchk_ctx.alloc_size >= info->size);
        g_memchk_ctx.alloc_size -= info->size;

        {
            cf_free_native((void*)info->location.file_name);
            cf_free_native((void*)info->location.function_name);
            cf_free_native(info);
        }
        cf_hashtbl_set(g_memchk_ctx.alloc_info, &p, sizeof(void*), CF_NULL_PTR);
    } else {
        cf_assert(CF_FALSE);
    }

    cf_mutex_unlock(&g_memchk_ctx.mutex);
}

#else
#define CF_MEMTRACE_MALLOC(p, size, file, function, line) (void*)0
#define CF_MEMTRACE_FREE(p, file, function, line) (void*)0
#endif



cf_void_t* cf_malloc_dbg(cf_size_t size,
                         cf_bool_t clear,
                         const cf_char_t* file,
                         const cf_char_t* function,
                         int line) {
    cf_void_t* p = malloc(size);
    if (clear) {
        cf_membzero(p, size);
    }
    CF_MEMTRACE_MALLOC(p, size, file, function, line);
    return p;
}

cf_void_t  cf_free_dbg(cf_void_t* addr, 
                       const cf_char_t* file,
                       const cf_char_t* function,
                        int line) {
    free(addr);
    
    CF_MEMTRACE_FREE(addr, file, function, line);
}

cf_void_t* cf_realloc_dbg(cf_void_t* addr, cf_size_t size) {
    return realloc(addr, size);
}

cf_void_t cf_memchk_init() {
#ifdef CF_MEMORY_DBG
    cf_membzero(&g_memchk_ctx, sizeof(g_memchk_ctx));
    g_memchk_ctx.inited = CF_TRUE;
    g_memchk_ctx.alloc_info = cf_hashtbl_new(32, CF_NULL_PTR);
    cf_mutex_init(&g_memchk_ctx.mutex, CF_NULL_PTR);
#endif
}

cf_bool_t cf_memchk_deinit_and_summary() {
    cf_bool_t is_success = CF_TRUE;
#ifdef CF_MEMORY_DBG
    cf_hashtbl_iter_t it;
    malloc_info* info;
    is_success = (g_memchk_ctx.alloc_size == 0);
    if (is_success) {
        printf("[memchk][leak] No leaks!\n");
    } else {
        printf("[memchk][leak] has leaks, leak size: %ud\n", (cf_uint32_t)g_memchk_ctx.alloc_size);
    }

    for (it = cf_hashtbl_iter_init(g_memchk_ctx.alloc_info);
        !cf_hashtbl_iter_end(it);
        it = cf_hashtbl_iter_next(it)) {
        info = cf_hashtbl_iter_value(it);
        if (info != CF_NULL_PTR) {
            printf("[%s@%d : %s] leak: %udbytes, stack: %s\n", info->location.file_name,
                info->location.line_number, info->location.function_name, (cf_uint32_t)info->size,
                info->info);
        }
    }
    
    cf_hashtbl_delete(g_memchk_ctx.alloc_info);
    cf_mutex_destroy(&g_memchk_ctx.mutex);
    cf_membzero(&g_memchk_ctx, sizeof(g_memchk_ctx));
#endif
    return is_success;
}

cf_void_t* cf_memcpy_s(cf_void_t* dst, cf_size_t dst_size, const cf_void_t* src, cf_size_t src_size) {
    if(dst_size < src_size) return CF_NULL_PTR;
    return memcpy(dst, src, src_size);
}

cf_void_t* cf_memset_s(cf_void_t* s, cf_size_t smax, cf_char_t c, cf_size_t rsize) {
    if(smax < rsize) return CF_NULL_PTR;
    return memset(s, c, rsize);
}

cf_void_t* cf_memchr(const cf_void_t* s, cf_char_t c, cf_size_t n) {
    cf_char_t* ss = (cf_char_t*)s;
    if(!s || !n) return CF_NULL_PTR;
    while(n > 0 && *ss != c) ss++, n--;
    return (n == 0 ? CF_NULL_PTR : ss);
}

cf_void_t cf_membzero(cf_void_t* dst, cf_size_t n) {
#ifdef __GLIBC__
    bzero(dst, n);
#else
    cf_memset_s(dst, n, 0, n);
#endif
}

static void* _allocator_new_default(void* nullable_allocator, cf_size_t size) {
    CF_UNUSED_VAR(nullable_allocator);
    return cf_malloc(size);
}

static void _allocator_delete_default(void* nullable_allocator, void* address) {
    CF_UNUSED_VAR(nullable_allocator);
    cf_free(address);
}


static cf_allocator_t _g_allocator = {
    .impl = CF_NULL_PTR,
    .new_fn = _allocator_new_default,
    .delete_fn = _allocator_delete_default
};

cf_allocator_t* _g_allocators_[CF_MAX_ALLOCATOR_CNT] = {
    &_g_allocator,
    CF_NULL_PTR,
};

cf_allocator_t* cf_allocator_default() {
    return &_g_allocator;
}

cf_bool_t cf_allocator_register(cf_allocator_id_t id, cf_allocator_t* allocator) {
    if (_g_allocators_[id] != CF_NULL_PTR || !CF_ALLOCATOR_ID_VALID(id)) {
        return CF_FALSE;
    }
    _g_allocators_[id] = allocator;
    return CF_TRUE;
}

cf_allocator_t* cf_allocator_get(cf_allocator_id_t id) {
    if (!CF_ALLOCATOR_ID_VALID(id)) {
        return CF_NULL_PTR;
    }
    return _g_allocators_[id];
}
