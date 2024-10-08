/****************************************************
* Copyright (c) 2018, tkorays <tkorays@hotmail.com>
* All rights reserved.
*
* @file    types.h
* @brief   CFAST types.
* @version 1.0
* @author  tkorays<tkorays@hotmail.com>
* @date    2018-1-3
****************************************************/
#ifndef __CF_TYPES_H__
#define __CF_TYPES_H__

#include <stddef.h>
#include <cf/config.h>

/**
 * @defgroup CF_TPES types
 * This is the types module, which contains all the type definitions used in CFAST.
 * @{
 */

#define CF_NULL_PTR 0
#define CF_VOID_PTR (void*)0
#define CF_TRUE 1
#define CF_FALSE 0
#define CF_MIN2(a, b) ((a) > (b) ? (b) : (a))
#define CF_MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CF_SWAP16(a) (cf_uint16_t)((((cf_uint16_t)(a) & 0xFF) << 8) | (((cf_uint16_t)(a) & 0xFF00) >> 8))
#define CF_SWAP32(a) ((((a) & 0xFF) << 24) | (((a) & 0xFF00) << 8) | (((a) & 0xFF0000) >> 8) | (((a) & 0xFF000000) >> 24))
#define CF_SWAP64(a)            \
            ( ( ((a) >> 56) & 0x00000000000000FFLL ) |       \
              ( ((a) >> 40) & 0x000000000000FF00LL ) |       \
              ( ((a) >> 24) & 0x0000000000FF0000LL ) |       \
              ( ((a) >>  8) & 0x00000000FF000000LL ) |       \
              ( ((a) <<  8) & 0x000000FF00000000LL ) |       \
              ( ((a) << 24) & 0x0000FF0000000000LL ) |       \
              ( ((a) << 40) & 0x00FF000000000000LL ) |       \
              ( ((a) << 56) & 0xFF00000000000000LL ) )
#define CF_ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define CF_PCHAR(s) ((cf_char_t*)(s))
#define CF_TYPE_CAST(t, v) ((t)(v))
#define CF_P2V_CAST(t, v) (*(t)(v))
#define CF_ARRAY_GET(cast_to, addr, size, index) CF_TYPE_CAST(cast_to, CF_TYPE_CAST(cf_uint8_t*, addr) +(index) * (size))
#define CF_ASCII_LF '\012'
#define CF_ASCII_CR '\015'

#define CF_UNUSED_VAR(var) ((void*)(var))

#define CF_MAX_INT16    32767
#define CF_MIN_INT16    (-32768)
#define CF_MAX_UINT16   65535
#define CF_MAX_INT32    0x7fffffff
#define CF_MAX_UINT32   0xffffffff 
#define CF_MIN_INT32    0x80000000
#define CF_MAX_INT64    9223372036854775807L
#define CF_MAX_UINT64   18446744073709551615UL
#define CF_MIN_INT64    (-CF_MAX_INT64 - 1L)

#define CF_CONTAINER_OF(ptr, type, member) ({			\
                                            const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
                                            (type *)( (char *)__mptr - CF_OFFSET_OF(type,member) );})

#define CF_OFFSET_OF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#ifdef CF_BIG_ENDIAN
#define cf_hton_u16(a) (a)
#define cf_hton_u32(a) (a)
#define cf_hton_u64(a) (a)
#define cf_ntoh_u16(a) (a)
#define cf_ntoh_u32(a) (a)
#define cf_ntoh_u64(a) (a)
#else
#define cf_hton_u16(a) CF_SWAP16(a)
#define cf_hton_u32(a) CF_SWAP32(a)
#define cf_hton_u64(a) CF_SWAP64(a)
#define cf_ntoh_u16(a) CF_SWAP16(a)
#define cf_ntoh_u32(a) CF_SWAP32(a)
#define cf_ntoh_u64(a) CF_SWAP64(a)
#endif

#define _CF_IN_
#define _CF_OUT_
#define _CF_INOUT_


typedef int             cf_int_t;
typedef unsigned int    cf_uint_t;
typedef char            cf_char_t;
typedef unsigned char   cf_uchar_t;
typedef char            cf_int8_t;
typedef unsigned char   cf_uint8_t;
typedef short           cf_int16_t;
typedef unsigned short  cf_uint16_t;
typedef int             cf_int32_t;
typedef unsigned int    cf_uint32_t;
/* long long is not supported by VC6 */
#if defined(_MSC_VER) && (_MSC_VER < 1300)
typedef __int64         cf_int64_t;
typedef unsigned __int64 cf_uint64_t;
#else
typedef long long       cf_int64_t;
typedef unsigned long long cf_uint64_t;
#endif
typedef void            cf_void_t;
typedef unsigned int    cf_id_t;        
typedef void*           cf_handle_t; 
typedef size_t          cf_size_t;
typedef unsigned int    cf_bool_t;
#if CF_WORDSIZE == 32
typedef int             cf_uintptr_t;
#else
typedef cf_int64_t      cf_uintptr_t; 
#endif
typedef cf_uint32_t     cf_errno_t;     /** errno */

typedef struct {
    void*       iov_base;
    cf_size_t   iov_len;
} cf_iovec_t;

/** @} */

#endif /* __CF_TYPES_H__ */
