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

CF_DECLS_BEGIN

typedef cf_uint32_t cf_time_t;
typedef cf_uint32_t cf_time_interval_t;
typedef cf_uint64_t cf_clock_t;

typedef struct {
    cf_time_t   tv_sec;
    cf_uint32_t tv_usec;
} cf_timeval_t;

typedef struct cf_datetime {
	int	sec;		/* seconds after the minute [0-60] */
	int	min;		/* minutes after the hour [0-59] */
	int	hour;	/* hours since midnight [0-23] */
	int	mday;	/* day of the month [1-31] */
	int	mon;		/* months since January [0-11] */
	int	year;	/* years since 1900 */
	int	wday;	/* days since Sunday [0-6] */
	int	yday;	/* days since January 1 [0-365] */
	//int	tm_isdst;	/* Daylight Savings Time flag */
	//long	tm_gmtoff;	/* offset from UTC in seconds */
	//char	*tm_zone;	/* timezone abbreviation */
    cf_time_t   e_sec; 
    cf_uint32_t e_usec;
} cf_timeinfo_t;

cf_clock_t cf_clock_get();
cf_uint32_t cf_clock_diff_ms(cf_clock_t next, cf_clock_t prev);
cf_void_t cf_time_sleep(cf_uint32_t ms);
cf_errno_t cf_time_now(cf_timeinfo_t* ti);
cf_errno_t cf_time_str(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size);
cf_errno_t cf_time_strYMDHMS(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size);



CF_DECLS_END

#endif /* __CF_TIME_H__ */
