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

typedef cf_uint64_t cf_time_t;
typedef cf_uint64_t cf_time_interval_t;


cf_time_t cf_time_now(cf_void_t);
cf_void_t cf_time_sleep(cf_time_interval_t interval);
cf_errno_t cf_time_ctime(cf_time_t t, cf_char_t* buff, cf_size_t size);
//cf_errno_t cf_time_strftime();

CF_DECLS_END

#endif /* __CF_TIME_H__ */
