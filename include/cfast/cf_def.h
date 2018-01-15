/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cf_def.h
 * @brief   CFAST基础的定义
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 ****************************************************/
#ifndef __CF_DEF_H__
#define __CF_DEF_H__

#include <stdint.h>

/* 操作系统定义 */
#ifdef _WIN32
#define CF_OS_WIN 
#else 
#define CF_OS_LINUX 
#endif

#define CF_NULL_PTR 0
#define CF_NULL (void*)0
#define CF_TRUE 1
#define CF_FALSE 0
#define CF_MIN(a, b) ((a) > (b) ? (b) : (a))
#define CF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CF_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define CF_PCHAR(s) ((cf_char_t*)(s))

typedef int             cf_int_t;
typedef unsigned int    cf_uint_t;
typedef char            cf_char_t;
typedef unsigned char   cf_uchar_t;
typedef char            cf_int8_t;
typedef unsigned char   cf_uint8_t;
typedef short           cf_int16_t;
typedef unsigned short  cf_uint16_t;
typedef int32_t         cf_int32_t;
typedef uint32_t        cf_uint32_t;  
typedef int64_t         cf_int64_t;
typedef uint64_t        cf_uint64_t;  
typedef void            cf_void_t;      /** void* */
typedef uint32_t        cf_id_t;        /** 用于表示id */
typedef void*           cf_handle_t;    /** 用于表示句柄 */
typedef uint32_t        cf_size_t;      /** 用于表示大小 */
typedef uint8_t         cf_bool_t;      /** bool型变量 */
typedef uintptr_t       cf_uintptr_t;   /** 指针 */
typedef int32_t         cf_errno_t;     /** errno */


#define CF_ERR_MODULE_COMMON 0
#define CF_ERRNO_OK             CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 0)
#define CF_ERRNO_NOK            CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 1)
#define CF_ERRNO_INVALID_PARAM  CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 2)
#define CF_ERRNO_MALLOC_ERROR   CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 3)
#define CF_ERRNO_STATE_ERROR    CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 4)
#define CF_ERRNO_SIZE_OVER      CF_MAKE_ERRNO(CF_ERR_MODULE_COMMON, 5)

#ifndef CF_API
#  ifdef _WIN32
#       define CF_OS_WIN 
#       if defined(CF_BUILD_SHARED) /* build dll */
#           define CF_API __declspec(dllexport)
#       elif !defined(CF_BUILD_STATIC) /* use dll */
#           define CF_API __declspec(dllimport)
#       else /* static library */
#         define CF_API
#       endif
#  else
#       define CF_OS_LINUX
#       if __GNUC__ >= 4
#           define CF_API __attribute__((visibility("default")))
#       else
#           define CF_API
#       endif
#  endif
#endif

typedef enum {
    CF_RET_SUCCESS = 0,
    CF_RET_FAIL,
    CF_RET_PARAM_ERR,
    CF_RET_MALLOC_ERR,
    CF_RET_NULL_PTR
} cf_ret_t;

#endif /* __CF_DEF_H__ */
