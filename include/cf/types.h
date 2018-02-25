/****************************************************
* Copyright (c) 2018, tkorays <tkorays@hotmail.com>
* All rights reserved.
*
* @file    types.h
* @brief   CFAST基础的定义
* @version 1.0
* @author  tkorays<tkorays@hotmail.com>
* @date    2018-1-3
****************************************************/
#ifndef __CF_TYPES_H__
#define __CF_TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <cf/config.h>

#define CF_NULL_PTR 0
#define CF_NULL (void*)0
#define CF_TRUE 1
#define CF_FALSE 0
#define CF_MIN2(a, b) ((a) > (b) ? (b) : (a))
#define CF_MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CF_SWAP16(a) (cf_uint16_t)((((cf_uint16_t)(a) & 0xFF) << 8) | (((cf_uint16_t)(a) & 0xFF00) >> 8))
#define CF_SWAP32(a) ((((a) & 0xFF) << 24) | (((a) & 0xFF00) << 8) | (((a) & 0xFF0000) >> 8) | (((a) & 0xFF000000) >> 24))
#define CF_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define CF_PCHAR(s) ((cf_char_t*)(s))
#define CF_TYPE_CAST(v, t) ((t)(v))
#define CF_ASCII_LF '\012'
#define CF_ASCII_CR '\015'

#define CF_MAX_INT32    0x7fffffff
#define CF_MAX_UINT32   UINT_MAX
#define CF_MIN_INT32    0x80000000
#define CF_MAX_INT64    9223372036854775807L
#define CF_MAX_UINT64   18446744073709551615UL
#define CF_MIN_INT64    (-CF_MAX_INT64 - 1L)


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
typedef size_t          cf_size_t;      /** 用于表示大小 */
typedef uint32_t        cf_bool_t;      /** bool型变量 */
typedef uintptr_t       cf_uintptr_t;   /** 指针 */
typedef int32_t         cf_errno_t;     /** errno */


#endif /* __CF_TYPES_H__ */
