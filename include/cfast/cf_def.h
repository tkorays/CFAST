/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    cf_def.h
 * @brief   CFAST基础的定义
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 *********************************************/
#ifndef __CF_DEF_H__
#define __CF_DEF_H__

#include <stdint.h>

/* 操作系统定义 */
#ifdef _WIN32
#define CF_OS_WIN 
#else 
#define CF_OS_LINUX 
#endif

#define CF_NULL (void*)0
#define CF_TRUE 1
#define CF_FALSE 0
#define CF_MIN(a, b) ((a) > (b) ? (b) : (a))
#define CF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CF_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

typedef uint32_t    cf_id_t;        /** 用于表示id */
typedef void*       cf_handle_t;    /** 用于表示句柄 */
typedef uint32_t    cf_size_t;      /** 用于表示大小 */
typedef uint8_t     cf_bool_t;      /** bool型变量 */
typedef uintptr_t   cf_ptr_t;       /** 指针 */

typedef enum {
    CF_SUCCESS = 0,
    CF_FAIL,
    CF_PARAM_ERR,
    CF_MALLOC_ERR,
    CF_NULL_PTR
} cf_ret_t;

#endif /* __CF_DEF_H__ */
