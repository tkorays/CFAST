/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cf_mem_if.h
 * @brief   Memory management.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-5
 ****************************************************/
#ifndef __CF_MEMORY_H__
#define __CF_MEMORY_H__

#include <cf/types.h>
#include <stdlib.h>

/**
 * @defgroup CF_MEMORY memory
 * Memory allocation and memory allocator.
 * This module provides functions for allocating, reallocating, and freeing memory, as well as functions for copying and setting memory.
 * It also defines an interface for a memory allocator, which allows for custom memory allocation strategies.
 * @{
 */

#ifdef CF_MEMORY_DBG
#   define cf_malloc(size) cf_malloc_dbg(size, CF_FALSE, __FILE__, __FUNCTION__, __LINE__)
#   define cf_free(p) cf_free_dbg(p, __FILE__, __FUNCTION__, __LINE__)
#   define cf_realloc(p, size) cf_realloc_dbg(p, size)
#   define cf_malloc_z(size) cf_malloc_dbg(size, CF_TRUE, __FILE__, __FUNCTION__, __LINE__)
#else 
#   define cf_malloc(size) malloc(size)
#   define cf_free(p) free(p)
#   define cf_realloc(p, size) realloc(p, size)
#   define cf_malloc_z(size) calloc(1, size)
#endif

#define cf_malloc_native(size) malloc(size)
#define cf_free_native(p) free(p)
#define cf_realloc_native(p, size) realloc(p, size)
#define cf_malloc_z_native(size) calloc(1, size)

CF_DECLS_BEGIN


/**
 * Allocate memory, debug version.
 * @param size       Memory size
 * @return           Address of memory.
 */
cf_void_t* cf_malloc_dbg(cf_size_t size,
                         cf_bool_t clear,
                         const cf_char_t* file,
                         const cf_char_t* function,
                         int line);

/**
 * Destory memory, debug version.
 * @param addr       Address of memory.
 */
cf_void_t  cf_free_dbg(cf_void_t* addr,
                       const cf_char_t* file,
                       const cf_char_t* function,
                       int line);

/**
 * Re-Allocate memory, debug version.
 * @param addr       Memory size
 * @param size       Size of realloc.
 * @return           Address of memory.
 */
cf_void_t* cf_realloc_dbg(cf_void_t* addr, cf_size_t size);

/**
 * @brief initialize a instance to check the memory problem
 * 
 * don't use this in production
 * 
 * @return cf_void_t 
 */
cf_void_t cf_memchk_init();

/**
 * @brief deinitialize the check and return success or not
 * 
 * @return cf_bool_t true for zero problems
 */
cf_bool_t cf_memchk_deinit_and_summary();

/**
 * Copy memory from destination(Safety).
 * @param dst       Destination memory.
 * @param dst_size  Memory size of destination.
 * @param src       Source memory.
 * @param src_size  Memory size of source.
 * @return          Address of destination.
 */
cf_void_t* cf_memcpy_s(cf_void_t* dst, cf_size_t dst_size, const cf_void_t* src, cf_size_t src_size);

/**
 * Set memory as char c(Safety).
 * @param s         Source memory.
 * @param smax      Memory size of source.
 * @param c         The char to be set.
 * @param rsize     Repeat number.
 * @return          Address of memory.
 */
cf_void_t* cf_memset_s(cf_void_t* s, cf_size_t smax, cf_char_t c, cf_size_t rsize);

/**
 * Find a character in memory.
 * @param s         Memory.
 * @param c         The char to be find.
 * @param n         Memory Length
 * @return          Address of memory.
 */
cf_void_t* cf_memchr(const cf_void_t* s, cf_char_t c, cf_size_t n);

/**
 * Fill the memroy with zeros.
 * @param s         Memory.
 * @param n         Memory size
 */
cf_void_t cf_membzero(cf_void_t* dst, cf_size_t n);


typedef void*(*cf_alloc_new_fn)(void* nullable_allocator, cf_size_t size);
typedef void(*cf_alloc_delete_fn)(void* nullable_allocator, void* address);

/**
 * @brief this is the interface for a memory allocator.
 * 
 */
typedef struct cf_allocator {
    void*               impl;       /** pointer of the allocator implementation */
    cf_alloc_new_fn     new_fn;     /** new function */
    cf_alloc_delete_fn  delete_fn;  /** delete function */
} cf_allocator_t;

typedef cf_uint32_t cf_allocator_id_t;
#define CF_MAX_ALLOCATOR_CNT    16
#define CF_ALLOCATOR_ID_BEGIN   0
#define CF_ALLOCATOR_ID_DEFAULT 0
#define CF_ALLOCATOR_ID_MPOOL   1
#define CF_ALLOCATOR_ID_END     ((CF_MAX_ALLOCATOR_CNT) - 1)
#define CF_ALLOCATOR_ID_VALID(id) ((id) >= (CF_ALLOCATOR_ID_BEGIN) && (id) <= (CF_ALLOCATOR_ID_END))
#define CF_ALLOCATOR_NONE       CF_NULL_PTR

cf_allocator_t* cf_allocator_default();
cf_bool_t cf_allocator_register(cf_allocator_id_t id, cf_allocator_t* allocator);
cf_allocator_t* cf_allocator_get(cf_allocator_id_t id);

typedef struct cf_mpool cf_mpool_t;
cf_bool_t cf_allocator_init_from_mpool(cf_allocator_t* self, cf_mpool_t* mpool);

CF_DECLS_END

/** @} */

#endif /* __CF_MEMORY_H__ */
