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
 * a linked list
 */
typedef struct cf_list {
    cf_size_t           count;      /*< number of list items */
    cf_void_t*          head;       /*< head of list */
    cf_void_t*          tail;       /*< tail of list */
} cf_list_t;

CF_DECLS_BEGIN

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
 * Initialize a list.
 *
 * @param self      this pointer
 * @return          true for success
 */
cf_bool_t cf_list_init(cf_list_t* self);

/**
 * Destroy a list.
 * 
 * @param self      this pointer
 */
cf_void_t cf_list_deinit(cf_list_t* self);

/**
 * Insert item to list.
 * @param li        The list.
 * @param data      Insert data item.
 * @param pos       Inset position, range from -N-1 to N.
 * @return          The result.
 */
cf_bool_t cf_list_insert(cf_list_t* li, cf_int32_t pos, cf_void_t* data);

/**
 * Remove item from list, NOT free data.
 * @param li        The list.
 * @param data      Data item address.
 * @param pos       Inset position, range from -N-1 to N.
 * @return          The result.
 */
cf_void_t* cf_list_erase(cf_list_t* li, cf_int32_t pos);


/**
 * get list data by position
 *
 * @param li        this pointer
 * @param pos       position of the item
 * @return          returned data
 */
cf_void_t* cf_list_get(cf_list_t* li, cf_int32_t pos);

/**
 * Get list head
 *
 * @param li        this pointer
 * @return          returned data
 */
cf_void_t* cf_list_head(cf_list_t* li);

/**
 * Get list tail
 *
 * @param li        this pointer
 * @return          returned data
 */
cf_void_t* cf_list_tail(cf_list_t* li);


/**
 * Init the iterator, return the first iterator.
 * @param li        The list.
 * @return          The result.
 */
cf_list_iter_t cf_list_iter_init(cf_list_t* li);

/**
 * Get next iterator.
 * @param it        The iterator.
 * @return          The next iterator.
 */
cf_list_iter_t  cf_list_iter_next(cf_list_iter_t it);

/**
 * Get data from iterator.
 * @param it        The iterator.
 * @return          The result.
 */
cf_void_t* cf_list_iter_data(cf_list_iter_t it);


#define cf_list_size(li) ((li)->count)

CF_DECLS_END

#endif /* __CF_LIST_H__ */
