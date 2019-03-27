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

typedef struct cf_datetime {
	cf_uint32_t	sec;		/* seconds after the minute [0-60] */
    cf_uint32_t	min;		/* minutes after the hour [0-59] */
    cf_uint32_t	hour;	/* hours since midnight [0-23] */
    cf_uint32_t	mday;	/* day of the month [1-31] */
    cf_uint32_t	mon;		/* months since January [0-11] */
    cf_uint32_t	year;	/* years since 1900 */
    cf_uint32_t	wday;	/* days since Sunday [0-6] */
    cf_uint32_t msec;
} cf_timeinfo_t;

cf_clock_t cf_clock_get();
cf_uint64_t cf_clock_diff_ms(cf_clock_t next, cf_clock_t prev);
cf_void_t cf_time_sleep(cf_uint32_t ms);
cf_errno_t cf_time_now(cf_timeinfo_t* ti);
cf_errno_t cf_time_str(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size);
cf_errno_t cf_time_strYMDHMS(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size);
cf_int32_t cf_day_of_year(const cf_timeinfo_t* ti);
cf_bool_t cf_time_is_valid(const cf_timeinfo_t* ti);



CF_DECLS_END

#endif /* __CF_TIME_H__ */
