/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    numerics.h
 * @brief   numerics library.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2023-2-14
 ****************************************************/
#ifndef __CF_NUMERICS_H__
#define __CF_NUMERICS_H__

#include "cf/types.h"

/**
 * @defgroup CF_UNWRAPPER unwrapper
 * Simple array
 * @{
 */

CF_DECLS_BEGIN

/**
 * unwarpper for uint8/uit16/uint32
 */
typedef struct {
    cf_int64_t      last_unwrapped; /** unwrapped value */
    cf_bool_t       has_value;      /** has last value */
    cf_uint64_t     last_value;     /** last value */
} cf_unwrapper_t;


/**
 * initialize a number unwrapper
 * @param self  this pointer
 */
void cf_unwrapper_init(cf_unwrapper_t* self);


/**
 * resest an unwrapper
 * @param   self    this pointer
 */
void cf_unwrapper_reset(cf_unwrapper_t* self);

/**
 * unwrap for uint8
 * @param   self    this pointer
 * @param   n       input number
 * @return  unwrapped number
 */
cf_int64_t cf_unwrapper_u8(cf_unwrapper_t* self, cf_uint8_t n);

/**
 * unwrap for uint16
 * @param   self    this pointer
 * @param   n       input number
 * @return  unwrapped number
 */
cf_int64_t cf_unwrapper_u16(cf_unwrapper_t* self, cf_uint16_t n);

/**
 * unwrap for uint32
 * @param   self    this pointer
 * @param   n       input number
 * @return  unwrapped number
 */
cf_int64_t cf_unwrapper_u32(cf_unwrapper_t* self, cf_uint32_t n);

CF_DECLS_END

/** @} */

#endif /* __CF_NUMERICS_H__ */

