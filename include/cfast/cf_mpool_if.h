/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cf_mpool_if.h
 * @brief   Memory pool.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-5
 ****************************************************/
#ifndef __CF_MPOOL_H__
#define __CF_MPOOL_H__

#include "cf_def.h"

/**
 * Default block size
 */
#define CF_MPOOL_DEF_BLK_SIZE 1024

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
 * @return           
 */
cf_void_t   cf_mpool_destroy(cf_mpool_t* pool);

/**
 * Allocate memory from a pool.
 * @param pool       The pointer of memory pool.
 * @param size       Size of allocated memory.
 * @return           Address of allocated memory.
 */
cf_void_t*  cf_mpool_alloc(cf_mpool_t* pool, cf_size_t size);

#endif /* __CF_MPOOL_H__ */
