/****************************************************
 * Copyright (c) 2021, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    algorithm.h
 * @brief   algorithm
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2022-09-07
 ****************************************************/
#ifndef __CF_ALGORITHM_H__
#define __CF_ALGORITHM_H__

#include <cf/config.h>
#include <cf/types.h>

/**
 * @defgroup CF_ALG algorithms
 *
 * some algorithms
 * @{
 */

CF_DECLS_BEGIN

typedef cf_bool_t(*cf_alg_equal_f)(cf_void_t* a, cf_void_t* b);
typedef cf_bool_t(*cf_alg_greater_to_f)(cf_void_t* a, cf_void_t* b);
typedef cf_bool_t(*cf_alg_lower_to_f)(cf_void_t* a, cf_void_t* b);

CF_DECLS_END

/** @} */

#endif /* __CF_ALGORITHM_H__ */
