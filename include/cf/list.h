/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    list.h
 * @brief   A double linked list.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 ****************************************************/
#ifndef __CF_LIST_H__
#define __CF_LIST_H__

#include <cf/types.h>

/**
 * Type of free function to free item data of a list.
 */
typedef void(*fn_cf_list_free)(void*);

/**
 * This structure represents a double linked list.
 */
typedef struct cf_list cf_list_t;

/**
 * Iterator of a list
 */
typedef cf_void_t* cf_list_iter_t;

/**
 * CF_LIST_POS_HEAD indicates the head of a list.
 */
#define CF_LIST_POS_HEAD 0

/**
 * CF_LIST_POS_TAIL indicates the tail of a list.
 */
#define CF_LIST_POS_TAIL -1

/**
 * Create a list.
 * @param fn_cf_list_free       Func for free data
 * @return                      The list.
 */
cf_list_t*  cf_list_create(fn_cf_list_free func);

/**
 * Insert item to list.
 * @param li        The list.
 * @param data      Insert data item.
 * @param pos       Inset position, range from -N-1 to N.
 * @return          The result.
 */
cf_errno_t cf_list_insert(cf_list_t* li, cf_void_t* data, cf_int32_t pos);

/**
 * Remove item from list, NOT free data.
 * @param li        The list.
 * @param data      Data item address.
 * @param pos       Inset position, range from -N-1 to N.
 * @return          The result.
 */
cf_errno_t cf_list_remove(cf_list_t* li, cf_void_t** data, cf_int32_t pos);

/**
 * Delete item from list, MAY free data.
 * @param li        The list.
 * @param pos       Inset position, range from -N-1 to N.
 * @param free_data Whethere to free data.
 * @return          The result.
 */
cf_errno_t cf_list_delete(cf_list_t* li, cf_int32_t pos, cf_bool_t free_data);

/**
 * Free the list.
 * @param li        The list.
 * @param free_data Whethere to free data.
 * @return          The result.
 */
cf_errno_t cf_list_destroy(cf_list_t* li, cf_bool_t free_data);

/**
 * Get the size of the list.
 * @param li        The list.
 * @return          The size.
 */
cf_size_t   cf_list_size(cf_list_t* li);

/**
 * Init the iterator, return the first iterator.
 * @param li        The list.
 * @param it        The iterator.
 * @return          The result.
 */
cf_errno_t cf_list_iter_init(cf_list_t* li, cf_list_iter_t* it);

/**
 * Get next iterator.
 * @param li        The iterator.
 * @return          The next iterator.
 */
cf_list_iter_t  cf_list_iter_next(cf_list_iter_t it);

/**
 * Get data from iterator.
 * @param it        The iterator.
 * @param data      Data.
 * @return          The result.
 */
cf_errno_t cf_list_iter_data(cf_list_iter_t it, cf_void_t** data);

#endif /* __CF_LIST_H__ */
