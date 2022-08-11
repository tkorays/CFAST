/****************************************************
 * Copyright (c) 2022, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    clock.h
 * @brief   clock is a high level time interface.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2022-8-11
 ****************************************************/
#ifndef __CFAST_CLOCK_H__
#define __CFAST_CLOCK_H__

#include <cf/types.h>

CF_DECLS_BEGIN

/**
 * @brief clock interface
 * 
 */
typedef struct {
    cf_int64_t      offset_ns;
} cf_clock_t;

/**
 * @brief Get the global clock instance.
 * 
 * @return cf_clock_t* 
 */
cf_clock_t* cf_clock_global();

/**
 * @brief Set the global clock intanse for test.
 * 
 * @param clock 
 */
void cf_clock_set_global(cf_clock_t* clock);

/**
 * @brief steady clock since the os' bootstrap.
 * 
 * @return cf_clock_t* 
 */
cf_clock_t* cf_clock_steady();

/**
 * @brief system's NTP time
 * 
 * @return cf_clock_t* 
 */
cf_clock_t* cf_clock_system();

/**
 * @brief initialize a clock
 * 
 * @param self 
 */
void cf_clock_init(cf_clock_t* self);

/**
 * @brief deinitialize a clock
 * 
 * @param self 
 */
void cf_clock_deinit(cf_clock_t* self);

/**
 * @brief get nanosecond
 * 
 * @param self 
 * @return cf_int64_t 
 */
cf_int64_t cf_clock_current_ns(cf_clock_t* self);

/**
 * @brief get millisecond
 * 
 * @param self 
 * @return cf_int64_t 
 */
cf_int64_t cf_clock_current_ms(cf_clock_t* self);

/**
 * @brief get second
 * 
 * @param self 
 * @return cf_int64_t 
 */
cf_int64_t cf_clock_current_s(cf_clock_t* self);

/**
 * @brief Get NTP time.
 * 
 * @param self 
 * @return cf_int64_t 
 */
cf_int64_t cf_clock_ntp_ms(cf_clock_t* self);


CF_DECLS_END

#endif /* __CFAST_CLOCK_H__ */
