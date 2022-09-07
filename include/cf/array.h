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

cf_bool_t cf_array_init(cf_array_t* self, cf_size_t el_size, cf_int_t capacity);
cf_void_t cf_array_deinit(cf_array_t* self);
cf_bool_t cf_array_insert(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);
cf_bool_t cf_array_erase(cf_array_t* self, int index);
cf_bool_t cf_array_set(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size);
cf_void_t* cf_array_get(cf_array_t* self, int index);
cf_int_t cf_array_find(cf_array_t* self, cf_void_t* elm, cf_size_t size, cf_alg_equal_f cmp);

#define cf_array_size(arr) ((arr)->el_count)
#define cf_array_clear(arr) do {    \
    (arr)->capacity = 0;            \
    (arr)->elm_size = 0;            \
    (arr)->elm_count = 0;           \
    (arr)->elm_start = CF_NULL_PTR; } while(0)

CF_DECLS_END

#endif /* ifndef __CF_ARRAY_H__ */
