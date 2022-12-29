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
 * a simple array.
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
 * init a array.
 *
 * @param self      this pointer
 * @param el_size   element size, not exceed 1024 bytes
 * @param capacity  initial capacity
 * @return          true for success
 */
cf_bool_t cf_array_init(cf_array_t* self, cf_size_t el_size, cf_int_t capacity);

/**
 * destroy array
 *
 * @param self      this pointer
 */
cf_void_t cf_array_deinit(cf_array_t* self);

/**
 * insert element to the array
 *
 * @param self      this pointer
 * @param index     insert index, CF_ARRAY_BEGIN_INDEX = 0, CF_ARRAY_END_INDEX = -1
 * @param elm       element address
 * @param size      size of element
 * @return          true for success
 */
cf_bool_t cf_array_insert(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);

/**
 * remove a element.
 *
 * @param self      this pointer
 * @param index     element index
 * @return          true for success
 */
cf_bool_t cf_array_erase(cf_array_t* self, int index);

/**
 * set the array element
 *
 * @param self      this pointer
 * @param index     element index
 * @param elm       element address
 * @param size      size of element
 * @return          true for success
 */
cf_bool_t cf_array_set(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);

/**
 * get array element by index
 *
 * @param self      this pointer
 * @param index     element index
 * @return          the returned element
 */
cf_void_t* cf_array_get(cf_array_t* self, int index);

/**
 * find element by a given comapre function
 *
 * @param self      this pointer
 * @param elm       element address 
 * @param size      size of element
 * @param cmp       compare function
 * @return          the found element index, -2 for CF_ARRAY_INVALID_INDEX
 */
cf_int_t cf_array_find(cf_array_t* self, cf_void_t* elm, cf_size_t size, cf_alg_equal_f cmp);

/**
 * reset the array and clear elements
 *
 * @param self      this pointer
 * @return          true for success, false for failed
 */
cf_bool_t cf_array_reset(cf_array_t* self);

/**
 * get array size
 */
#define cf_array_size(arr) ((arr)->elm_count)

CF_DECLS_END

#endif /* ifndef __CF_ARRAY_H__ */
