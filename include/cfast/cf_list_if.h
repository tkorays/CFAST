/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    cf_list_if.h
 * @brief   CFAST的链表对外接口
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 *********************************************/
#ifndef __CF_LIST_IF_H__
#define __CF_LIST_IF_H__

#include "cf_def.h"

typedef void(*fn_cf_list_free)(void*);
typedef struct _cf_list {
    cf_size_t           number;     /*< 链表个数计数 */
    cf_void_t*          head;       /*< 链表头部 */
    cf_void_t*          tail;       /*< tail */
    fn_cf_list_free     fn_free;    /*< 链表释放指针 */
} cf_list_t;

typedef cf_void_t* cf_list_iter_t;

#define CF_LIST_POS_HEAD 0
#define CF_LIST_POS_TAIL -1


cf_list_t*  cf_list_new(fn_cf_list_free func);
cf_ret_t    cf_list_insert(cf_list_t* li, cf_void_t* data, cf_int32_t pos);
cf_ret_t    cf_list_remove(cf_list_t* li, cf_void_t* data, cf_int32_t pos);
cf_ret_t    cf_list_delete(cf_list_t* li, cf_int32_t pos, cf_bool_t free_data);
cf_ret_t    cf_list_free(cf_list_t* li, cf_bool_t free_data);
cf_ret_t    cf_list_iter_init(cf_list_t* li, cf_list_iter_t* it);
cf_list_iter_t  cf_list_iter_next(cf_list_iter_t it);
cf_ret_t    cf_list_iter_data(cf_list_iter_t it, cf_void_t* data);

#endif /* __CF_LIST_IF_H__ */
