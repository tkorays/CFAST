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

/**
 * @brief function pointer type for comparison
 * 
 * @param a pointer to the first element
 * @param b pointer to the second element
 * @return 0 if equal, -1 if a < b, 1 if a > b
 */
typedef cf_int_t(*cf_alg_cmp_f)(const cf_void_t* a, const cf_void_t* b);

/**
 * @brief function pointer type for equal comparison
 * 
 * @param a pointer to the first element
 * @param b pointer to the second element
 * @return cf_bool_t true if equal, false otherwise
 */
typedef cf_bool_t(*cf_alg_equal_f)(const cf_void_t* a, const cf_void_t* b);

/**
 * @brief function pointer type for greater comparison
 * 
 * @param a pointer to the first element
 * @param b pointer to the second element
 * @return cf_bool_t true if a is greater than b, false otherwise
 */
typedef cf_bool_t(*cf_alg_greater_to_f)(const cf_void_t* a, const cf_void_t* b);

/**
 * @brief function pointer type for lower comparison
 * 
 * @param a pointer to the first element
 * @param b pointer to the second element
 * @return cf_bool_t true if a is lower than b, false otherwise
 */
typedef cf_bool_t(*cf_alg_lower_to_f)(const cf_void_t* a, const cf_void_t* b);

CF_DECLS_END

/** @} */

#endif /* __CF_ALGORITHM_H__ */
