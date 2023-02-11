/****************************************************
* Copyright (c) 2022, tkorays <tkorays@hotmail.com>
* All rights reserved.
*
* @file    location.h
* @brief   code location.
* @version 1.0
* @author  tkorays<tkorays@hotmail.com>
* @date    2022-8-4
****************************************************/
#ifndef __CFAST_LOCATION_H__
#define __CFAST_LOCATION_H__

#include <cf/types.h>

/**
 * @defgroup CF_LOCATION location
 * file location
 * @{
 */

CF_DECLS_BEGIN

typedef struct {
    const char* file_name;      //< filename of this location
    const char* function_name;  //< function name of this location
    int         line_number;    //< line number of this location
} cf_location_t;

/**
 * @brief define a location.
 * 
 */
#define cf_location_define(location) cf_location_t location;\
    do { (location).file_name = __FILE__; \
        (location).function_name = __FUNCTION__; (location).line_number = __LINE__; } while(0)

#define cf_location_set(self) do { (self)->file_name = __FILE__; \
        (self)->function_name = __FUNCTION__; (self)->line_number = __LINE__; } while(0)

CF_DECLS_END

/** @} */

#endif /* __CFAST_LOCATION_H__ */
