/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    event.h
 * @brief   event
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2023-2-11
 ****************************************************/
#ifndef __CF_EVENT_H__
#define __CF_EVENT_H__

#include <cf/types.h>

/**
 * @defgroup CF_EVENT event
 * system independent event
 * @{
 */

CF_DECLS_BEGIN

/**
 * @brief a cross-platform event
 * 
 */
typedef struct cf_event cf_event_t;

/**
 * @brief create new event
 * 
 * @param manual_reset 
 * @param signaled 
 * @return cf_event_t* 
 */
cf_event_t* cf_event_new(
    cf_bool_t manual_reset,
    cf_bool_t signaled);

/**
 * @brief destroy event instance
 * 
 * @param self 
 */
void cf_event_delete(cf_event_t* self);

/**
 * @brief set event
 * 
 * @param self this pointer
 */
void cf_event_set(cf_event_t* self);

/**
 * @brief reset event
 * 
 * @param self this pointer
 */
void cf_event_reset(cf_event_t* self);

/**
 * @brief wait a event
 * 
 * @param self          this pointer
 * @param timeout_ms    wait timeout
 * @return cf_bool_t    success or not
 */
cf_bool_t cf_event_wait(cf_event_t* self, int timeout_ms);

CF_DECLS_END

/** @} */

#endif /* __CF_EVENT_H__ */

