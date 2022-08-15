/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    time.h
 * @brief   Time.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-17
 ****************************************************/
#ifndef __CF_TIME_H__
#define __CF_TIME_H__

#include <cf/types.h>

#define CF_IS_LEAP_YEAR(y) (((y) % 4 == 0) && ((y) % 100 != 0))

CF_DECLS_BEGIN

typedef cf_uint32_t cf_time_t;
typedef cf_uint32_t cf_time_interval_t;
typedef cf_uint64_t cf_clock_t;

typedef struct {
    cf_time_t   tv_sec;
    cf_uint32_t tv_usec;
} cf_timeval_t;

typedef struct {
    cf_uint32_t	year : 12;
    cf_uint32_t	month : 4;
    cf_uint32_t	day : 5;
    cf_uint32_t	hour : 5;
    cf_uint32_t week_day : 5;
    cf_uint32_t utc : 1;

    cf_uint8_t	minute;
	cf_uint8_t	second;
    cf_uint16_t millisecond;
} cf_datetime_t;

/**
 * @brief Sleep in current thread
 * 
 * @param ms sleep duration
 * @return cf_void_t 
 */
cf_void_t cf_time_sleep(cf_uint32_t ms);

/**
 * @brief return the current timestamp
 * 
 * @param dt [in] param
 * @return cf_void_t 
 */
cf_bool_t cf_datetime_now(cf_datetime_t* dt);

/**
 * @brief return the current timestamp in millisecond
 * 
 * @return cf_int64_t 
 */
cf_int64_t cf_datetime_timestamp();

/**
 * @brief calculate day of year
 * 
 * @param dt datetime instance
 * @return cf_int32_t 
 */
cf_int32_t cf_datetime_day_of_year(cf_datetime_t* dt);


CF_DECLS_END

#endif /* __CF_TIME_H__ */
