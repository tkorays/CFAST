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
#include <cf/algorithm.h>

/**
 * @defgroup CF_LIST list
 * double linked list
 * @{
 */

CF_DECLS_BEGIN

/**
 * node in a linked list
 * NOTE: don't use structure's members directly, use functions instead.
 */
typedef struct cf_list_node {
    cf_void_t*              data;   /** node data */
    struct cf_list_node*    prev;   /** prev node */
    struct cf_list_node*    next;   /** next node */
} cf_list_node_t;

/**
 * a linked list
 * we use the `data` filed in cf_list_node_t to count items.
 */
typedef cf_list_node_t  cf_list_t;


/**
 * Iterator of a list
 */
typedef struct cf_list_node* cf_list_iter_t;


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
 * @brief insert data before a iterator
 * 
 * @param self  this pointer
 * @param iter  iterator
 * @param data  inserted data
 * @return cf_void_t 
 */
cf_void_t cf_list_insert_before(cf_list_t* self, cf_list_iter_t iter, void* data);

/**
 * @brief insert data after a iterator
 * 
 * @param self  this pointer
 * @param iter  iterator
 * @param data  inserted data
 * @return cf_void_t 
 */
cf_void_t cf_list_insert_after(cf_list_t* self, cf_list_iter_t iter, void* data);

/**
 * @brief erase a node in list
 * 
 * @param self  this pointer
 * @param iter  iterator
 * @return cf_void_t 
 */
cf_void_t cf_list_erase(cf_list_t* self, cf_list_iter_t iter);

/**
 * @brief insert data in the end
 * 
 * @param self  this pointer
 * @param data  inserted data
 * @return cf_void_t 
 */
CF_FORCE_INLINE cf_void_t cf_list_push_back(cf_list_t* self, void* data) {
    cf_list_insert_after(self, self->prev, data);
}

/**
 * @brief insert data in the font
 * 
 * @param self  this pointer
 * @param data  inserted data
 * @return cf_void_t 
 */
CF_FORCE_INLINE cf_void_t cf_list_push_front(cf_list_t* self, void* data) {
    if (self->prev == self->next) {
        cf_list_insert_after(self, self, data);
    } else {
        cf_list_insert_before(self, self->next, data);
    }
}

/**
 * Get list head
 *
 * @param li        this pointer
 * @return          returned data
 */
CF_FORCE_INLINE cf_void_t* cf_list_head(cf_list_t* self) {
    if (self->next != self) {
        return self->next->data;
    }
    return CF_NULL_PTR;
}

/**
 * Get list tail
 *
 * @param li        this pointer
 * @return          returned data
 */
CF_FORCE_INLINE cf_void_t* cf_list_tail(cf_list_t* self) {
    if (self->prev != self) {
        return self->prev->data;
    }
    return CF_NULL_PTR;
}

/**
 * Init the iterator, return the first iterator.
 * @param li        The list.
 * @return          The result.
 */
CF_FORCE_INLINE cf_list_iter_t cf_list_iter_init(cf_list_t* self) {
    return self->next;
}

/**
 * Get next iterator.
 * @param it        The iterator.
 * @return          The next iterator.
 */
CF_FORCE_INLINE cf_list_iter_t cf_list_iter_next(cf_list_iter_t it) {
    return it->next;
}

/**
 * it the iterator end?
 * 
 * @param it            The iterator
 * @return cf_bool_t    CF_TRUE on end    
 */
CF_FORCE_INLINE cf_bool_t cf_list_iter_end(cf_list_t* self, cf_list_iter_t it) {
    return it == self ? CF_TRUE : CF_FALSE;
}

/**
 * Get data from iterator.
 * @param it        The iterator.
 * @return          The result.
 */
CF_FORCE_INLINE cf_void_t* cf_list_iter_data(cf_list_iter_t it) {
    return it->data;
}

/**
 * @brief find data from list, and return the iterator
 * 
 * @param self  this pointer
 * @param data  data to be found
 * @param eqf   compare function
 * @return cf_list_iter_t NULL on failed
 */
cf_list_iter_t cf_list_find(cf_list_t* self, void* data, cf_alg_equal_f eqf);

/**
 * @brief return the list size
 * 
 * @param self  this pointer
 * @return cf_size_t    list size
 */
CF_FORCE_INLINE cf_size_t cf_list_size(cf_list_t* self) {
    return CF_TYPE_CAST(cf_size_t, CF_TYPE_CAST(cf_uintptr_t, self->data));
}

CF_DECLS_END

/** @} */

#endif /* __CF_LIST_H__ */
