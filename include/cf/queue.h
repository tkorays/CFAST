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
 * @defgroup CF_QUEUE queue
 * double ended queue
 * @{
 */

CF_DECLS_BEGIN

/**
 * Queue.
 */
typedef struct cf_que_s cf_que_t;

cf_que_t*   cf_que_create(cf_size_t max_size);
cf_errno_t  cf_que_destroy(cf_que_t* que);
cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size);
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size);


/**
 * @brief item size can not exceed 10k for lite queue
 * 
 */
#define CF_LITE_QUEUE_MAX_ITEM_SIZE (1024*10)

#define CF_LITE_QUEUE_INIT_CAPACITY (20)
#define CF_LITE_QUEUE_MAX_CAPACITY (65535)

/**
 * @brief A lite queue store all same size items in the queue.
 * 
 */
typedef struct {
    void*       items;      //< address of the first item
    cf_size_t   elm_size;     //< all items have the same size
    cf_size_t   capacity;   //< max items this queue can hold
    cf_uint_t   head;       //< head index of the queue
    cf_uint_t   tail;       //< tail index of this queue
    cf_size_t   count;      //< count of items
} cf_lite_queue_t;

/**
 * @brief initialize a lite queue with item size and initial capacity.
 * 
 * @param self self pointer
 * @param elm_size item size of every item
 * @param init_capacity initial capacity of this queue
 * @return cf_bool_t 
 */
cf_bool_t cf_lite_queue_init(cf_lite_queue_t* self, cf_size_t elm_size, cf_size_t init_capacity);

/**
 * @brief deinitialize lite queue
 * 
 * @param self self pointer
 */
void cf_lite_queue_deinit(cf_lite_queue_t* self);

/**
 * @brief return the front item
 * 
 * @param self self pointer
 * @return void* item pointer
 */
void* cf_lite_queue_front(cf_lite_queue_t* self);

/**
 * @brief return the back item
 * 
 * @param self self pointer
 * @return void* item pointer
 */
void* cf_lite_queue_back(cf_lite_queue_t* self);

/**
 * @brief push a new item to front
 * 
 * @param self self pointer
 * @param data data
 * @return cf_bool_t success/fail
 */
cf_bool_t cf_lite_queue_push_back(cf_lite_queue_t* self, void* data);

/**
 * @brief push a new item to back
 * 
 * @param self self pointer
 * @param data data
 * @return cf_bool_t success/fail
 */
cf_bool_t cf_lite_queue_push_front(cf_lite_queue_t* self, void* data);

/**
 * @brief pop up item from front
 * 
 * @param self 
 * @return void* 
 */
void* cf_lite_queue_pop_front(cf_lite_queue_t* self);

/**
 * @brief pop up item from back
 * 
 * @param self 
 * @return void* 
 */
void* cf_lite_queue_pop_back(cf_lite_queue_t* self);

/**
 * @brief clear all items
 * 
 * @param self 
 */
void cf_lite_queue_clear(cf_lite_queue_t* self);

/**
 * @brief item size of this queue
 * 
 */
#define cf_lite_queue_item_size(self) ((self)->elm_size)

/**
 * @brief is this queue empty
 * 
 */
#define cf_lite_queue_empty(self) ((self)->count == 0)

/**
 * @brief item count
 * 
 */
#define cf_lite_queue_count(self) ((self)->count)


CF_DECLS_END

/** @} */

#endif /* __CF_QUEUE_H__ */
