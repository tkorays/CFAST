/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    mpool.h
 * @brief   Memory pool.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-5
 ****************************************************/
#ifndef __CF_MPOOL_H__
#define __CF_MPOOL_H__

#include <cf/types.h>

/**
 * @defgroup CF_MPOOL memory pool
 * a simple memory pool
 * @{
 */

CF_DECLS_BEGIN

/**
 * Default block size
 */
#define CF_MPOOL_DEF_BLK_SIZE 1024

/**
 * Statistics for memory pool.
 */
typedef struct {
    cf_size_t   blksize;    /** single block size */
    cf_size_t   blknum;     /** blocks */
    cf_size_t   lgblknum;   /** large blocks */
    cf_size_t   used;       /** used memory size */
    cf_size_t   unused;     /** unused momory size */
} cf_mpool_stat_t;

/**
 * Memory pool.
 */
typedef struct cf_mpool cf_mpool_t;

/**
 * Create a memory pool.
 * @param blk_size   Block size, 1024 is suggested.
 * @return           The pointer of memory pool.
 */
cf_mpool_t* cf_mpool_create(cf_size_t blk_size);

/**
 * Destroy memory pool.
 * @param pool       The pointer of memory pool.
 */
cf_void_t   cf_mpool_destroy(cf_mpool_t* pool);

/**
 * Allocate memory from a pool.
 * @param pool       The pointer of memory pool.
 * @param size       Size of allocated memory.
 * @return           Address of allocated memory.
 */
cf_void_t*  cf_mpool_alloc(cf_mpool_t* pool, cf_size_t size);

/**
 * Get statistics for memory pool.
 * @param pool      The pointer of memory pool.
 * @param stat      Statistics.
 */
cf_void_t   cf_mpool_get_stat(cf_mpool_t* pool, cf_mpool_stat_t* stat);

CF_DECLS_END

/** @} */

#endif /* __CF_MPOOL_H__ */
