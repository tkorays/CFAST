/****************************************************
 * Copyright (c) 2021, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    array.h
 * @brief   array
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2022-08-28
 ****************************************************/
#ifndef __CF_ARRAY_H__
#define __CF_ARRAY_H__

#include <cf/types.h>
#include <cf/algorithm.h>

/**
 * @defgroup CF_ARRAY array
 * Simple array
 * @{
 */

/**
 * @brief a simple array.
 */
typedef struct cf_array {
    cf_size_t   capacity;   /* capacity of this array */
    cf_size_t   elm_size;   /* bytes of each element, should be lower to 1024 bytes */
    cf_size_t   elm_count;  /* count for elements */
    cf_void_t*  elm_start;  /* start memory for elements */
} cf_array_t;

CF_DECLS_BEGIN

#define CF_ARRAY_BEGIN_INDEX 0
#define CF_ARRAY_END_INDEX -1
#define CF_ARRAY_INVALID_INDEX -2

/**
 * Initialize an array.
 *
 * @param self      Pointer to the array to be initialized
 * @param el_size   Size of each element in bytes, should not exceed 1024 bytes
 * @param capacity  Initial capacity of the array
 * @return          True for success, false for failure
 */
cf_bool_t cf_array_init(cf_array_t* self, cf_size_t el_size, cf_int_t capacity);

/**
 * Deinitialize an array.
 *
 * @param self      Pointer to the array to be deinitialized
 */
cf_void_t cf_array_deinit(cf_array_t* self);

/**
 * Insert an element into the array.
 *
 * @param self      Pointer to the array
 * @param index     Index at which to insert the element, CF_ARRAY_BEGIN_INDEX = 0, CF_ARRAY_END_INDEX = -1
 * @param elm       Pointer to the element to be inserted
 * @param size      Size of the element in bytes
 * @return          True for success, false for failure
 */
cf_bool_t cf_array_insert(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);

/**
 * Remove an element from the array.
 *
 * @param self      Pointer to the array
 * @param index     Index of the element to be removed
 * @return          True for success, false for failure
 */
cf_bool_t cf_array_erase(cf_array_t* self, int index);

/**
 * Set the value of an element in the array.
 *
 * @param self      Pointer to the array
 * @param index     Index of the element to be set
 * @param elm       Pointer to the new value of the element
 * @param size      Size of the element in bytes
 * @return          True for success, false for failure
 */
cf_bool_t cf_array_set(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);

/**
 * Get the value of an element in the array.
 *
 * @param self      Pointer to the array
 * @param index     Index of the element to be retrieved
 * @return          Pointer to the retrieved element
 */
cf_void_t* cf_array_get(cf_array_t* self, int index);

/**
 * Find an element in the array using a given comparison function.
 *
 * @param self      Pointer tothe array
 * @param elm       Pointer to the element to be found
 * @param size      Size of the element in bytes
 * @param cmp       Comparison function to be used for finding the element
 * @return          Index of the found element, or CF_ARRAY_INVALID_INDEX (-2) if not found
 */
cf_int_t cf_array_find(cf_array_t* self, cf_void_t* elm, cf_size_t size, cf_alg_equal_f cmp);

/**
 * Reset the array and clear all elements.
 *
 * @param self      Pointer to the array to be reset
 * @return          True for success, false for failure
 */
cf_bool_t cf_array_reset(cf_array_t* self);

/**
 * Get the size of the array.
 *
 * @param arr       Pointer to the array
 * @return          Number of elements in the array
 */
#define cf_array_size(arr) ((arr)->elm_count)


CF_DECLS_END

/** @} */

#endif /* ifndef __CF_ARRAY_H__ */
