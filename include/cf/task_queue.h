/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    task_queue.h
 * @brief   task queue
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2023-2-11
 ****************************************************/
#ifndef __CF_TASK_QUEUE_H__
#define __CF_TASK_QUEUE_H__

#include "cf/types.h"

/**
 * @defgroup CF_TASK_QUEUE task queue
 * execute task asynchronously
 * @{
 */

CF_DECLS_BEGIN

/**
 * @brief task queue that asynchronosusly executes tasks
 * 
 */
typedef struct cf_task_queue cf_task_queue_t;

/**
 * @brief task function
 * 
 */
typedef void(*CF_FN_TASK_QUEUE_RUN)(void* data);

typedef struct {
    CF_FN_TASK_QUEUE_RUN    run;    /** executed function by task queue */
    void*                   data;   /** this memory should be managed by user */
} cf_task_queue_task_t;

/**
 * @brief create a new task queue with name and priority
 * 
 * @param name      name of this task
 * @param priority  priority of this task queue
 * @return cf_task_queue_t* 
 */
cf_task_queue_t* cf_task_queue_new(const char* name, int priority);

/**
 * @brief destroy a task queue
 * 
 * @param self this pointer
 */
void cf_task_queue_delete(cf_task_queue_t* self);

/**
 * @brief post an asynchronous task to task queue
 * 
 * @param self  this pointer
 * @param task  task to be executed
 */
void cf_task_queue_post(
    cf_task_queue_t* self,
    cf_task_queue_task_t task);

/**
 * @brief post a delayed task to task queue
 * 
 * @param self  this pointer
 * @param task  task to be executed
 * @param ms    delayed time
 */
void cf_task_queue_post_delayed(
    cf_task_queue_t* self,
    cf_task_queue_task_t task,
    int ms);

CF_DECLS_END

/** @} */

#endif /* __CF_TASK_QUEUE_H__ */
