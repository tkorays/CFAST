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
 * @brief A cross-platform event that can be used to signal and wait for events.
 * 
 * This is a system independent event implementation that can be used to signal and wait for events.
 * It provides functions to create, destroy, set, reset and wait for events.
 * 
 * @note This implementation is based on the Win32 event object and the POSIX condition variable.
 * 
 * @see cf_event_new
 * @see cf_event_delete
 * @see cf_event_set
 * @see cf_event_reset
 * @see cf_event_wait
 */
typedef struct cf_event cf_event_t;

/**
 * @brief create a new event instance
 * 
 * @param manual_reset  if true, the event needs to be manually reset
 * @param signaled      if true, the event is initially signaled
 * @return cf_event_t*  a pointer to the new event instance
 */
cf_event_t* cf_event_new(
    cf_bool_t manual_reset,
    cf_bool_t signaled);

/**
 * @brief destroy an event instance
 * 
 * @param self  a pointer to the event instance to destroy
 */
void cf_event_delete(cf_event_t* self);

/**
 * @brief set an event
 * 
 * @param self  a pointer to the event instance to set
 */
void cf_event_set(cf_event_t* self);

/**
 * @brief reset an event
 * 
 * @param self  a pointer to the event instance to reset
 */
void cf_event_reset(cf_event_t* self);

/**
 * @brief wait for an event
 * 
 * @param self          a pointer to the event instance to wait for
 * @param timeout_ms    the timeout in milliseconds
 * @return cf_bool_t    true if the event was signaled, false if the timeout was reached
 */
cf_bool_t cf_event_wait(cf_event_t* self, int timeout_ms);

CF_DECLS_END

/** @} */

#endif /* __CF_EVENT_H__ */

