/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 *
 * @file    config.h
 * @brief   Configuration for CFAST.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-2-10
 ****************************************************/
#ifndef __CF_CONFIG_H__
#define __CF_CONFIG_H__

/** Public API wrap for c++ compilers. */
#ifdef __cplusplus
#   define CF_DECLS_BEGIN extern "C" {
#   define CF_DECLS_END }
#else
#   define CF_DECLS_BEGIN 
#   define CF_DECLS_END 
#endif

#ifndef CF_API
#  ifdef _WIN32
#       define CF_OS_WIN 1
#       if defined(CF_BUILD_SHARED) /* build dll */
#           define CF_API __declspec(dllexport)
#       elif !defined(CF_BUILD_STATIC) /* use dll */
#           define CF_API __declspec(dllimport)
#       else /* static library */
#         define CF_API
#       endif
#  else
#       define CF_OS_LINUX 1

#       ifdef __APPLE__
#           include "TargetConditionals.h"
#           if TARGET_IPHONE_SIMULATOR
#               define CF_OS_IPHONE_SIM 1
#           elif TARGET_OS_IPHONE
#               define CF_OS_IPHONE 1
#           elif TARGET_OS_MAC
#               define CF_OS_MAC 1
#           else
#               error "Unknown Mac Platform!"
#           endif
#       endif

#       ifdef __ANDROID__
#           define CF_OS_ANDROID 1
#       endif

#       if __GNUC__ >= 4
#           define CF_API __attribute__((visibility("default")))
#       else
#           define CF_API
#       endif
#  endif
#endif

/**
 * API definations.
 * not used 
 */
#if !defined(CF_OS_WIN)
#   define CF_DECLARE(type) type
#   define CF_DECLARE_C(type) type
#elif defined CF_BUILD_STATIC
#   define CF_DECLARE(type) type __stdcall
#   define CF_DECLARE_C(type) type __cdecl
#elif (defined CF_BUILD_SHARED) && (defined CF_BUILD_EXPORT)
#   define CF_DECLARE(type) __declspec(dllexport) type __stdcall
#   define CF_DECLARE_C(type) __declspec(dllexport) type __cdecl
#elif (defined CF_BUILD_SHARED) && (defined CF_BUILD_IMPORT)
#   define CF_DECLARE(type) __declspec(dllimport) type __stdcall
#   define CF_DECLARE_C(type) __declspec(dllimport) type __cdecl
#else 
#   define CF_DECLARE(type) type
#   define CF_DECLARE_C(type) type
#endif

#define CF_WORDSIZE 32
/* gcc */
#if (defined __WORDSIZE) && (__WORDSIZE == 64)
#    undef CF_WORDSIZE
#    define CF_WORDSIZE 64
#endif
#if (defined __LP64__) || (defined __x86_64__) || (defined __amd64) || (defined __WIN64)
#    undef CF_WORDSIZE
#    define CF_WORDSIZE 64
#endif


#ifndef CF_DEBUG
#  ifndef NDEBUG
#    define CF_DEBUG 1
#  else
#    define CF_DEBUG 0
#  endif
#endif

#if (defined CF_DEBUG) && (CF_DEBUG != 0)
#  define CF_HAS_ASSERT 1
#endif

#define CF_FMT_SSZIE_T  "ld"
#define CF_FMT_SIZE_T   "lu"
#ifdef CF_OS_WIN
#   define CF_FMT_INT64_T   "I64d"
#   define CF_FMT_UINT64_T  "I64u"
#   define CF_FMT_UINT64_T_HEX  "I64x"
#else
#   define CF_FMT_INT64_T  "lld"
#   define CF_FMT_UINT64_T "llu"
#   define CF_FMT_UINT64_T_HEX "llx"
#endif

#define CF_MAX_PATH_SIZE 256
#ifdef CF_OS_WIN
#   define CF_PATH_SPLIT_CHAR  '\\'
#   define CF_PATH_SPLIT_STR   "\\"
#else
#   define CF_PATH_SPLIT_CHAR  '/'
#   define CF_PATH_SPLIT_STR   "/"
#endif

#define CF_VERSION_NUM_MAJOR 1
#define CF_VERSION_NUM_MINOR 0
#define CF_VERSION_NUM ((CF_VERSION_MAJOR << 16) | CF_VERSION_MINOR)

CF_DECLS_BEGIN

const char* cf_get_version();

CF_DECLS_END

#endif /* __CF_CONFIG_H__ */
