/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    queue.h
 * @brief   A light queue.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-17
 ****************************************************/
#ifndef __CF_QUEUE_H__
#define __CF_QUEUE_H__

#include <cf/types.h>

/**
 * Queue.
 */
typedef struct cf_que_s cf_que_t;

cf_que_t*   cf_que_create(cf_size_t max_size);
cf_errno_t  cf_que_destroy(cf_que_t* que);
cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size);
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size);

#endif /* __CF_QUEUE_H__ */
