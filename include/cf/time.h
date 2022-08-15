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

typedef struct {
    cf_uint32_t	year : 12;
    cf_uint32_t	month : 4;
    cf_uint32_t	day : 5;
    cf_uint32_t	hour : 5;
    cf_uint32_t week_day : 5;
    cf_uint32_t utc : 1;

    cf_uint32_t	minute : 8;
	cf_uint32_t	second : 8;
    cf_uint32_t millisecond : 16;

    cf_int64_t timestamp;
} cf_datetime_t;

typedef struct {
    cf_uint32_t	years : 12;
    cf_uint32_t	months : 4;
    cf_uint32_t	days : 5;
    cf_uint32_t	hours : 5;
    cf_uint32_t minutes : 6;

	cf_uint32_t	seconds : 6;
    cf_uint32_t milliseconds : 10;
    cf_uint32_t delta_ms : 16;
} cf_timedelta_t;

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
 * @brief time diff
 * 
 * @param dt1 datetime 1
 * @param dt2 datetime 2
 * @param delta timedelta between 1 and 2
 */
void cf_datetime_diff(const cf_datetime_t* dt1, const cf_datetime_t* dt2, cf_timedelta_t* delta);

/**
 * @brief return the current timestamp in millisecond
 * 
 * @return cf_int64_t 
 */
#define cf_datetime_timestamp(dt) ((dt)->timestamp)

/**
 * @brief calculate day of year
 * 
 * @param dt datetime instance
 * @return cf_int32_t 
 */
cf_int32_t cf_datetime_day_of_year(const cf_datetime_t* dt);


CF_DECLS_END

#endif /* __CF_TIME_H__ */
