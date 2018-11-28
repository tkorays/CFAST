/****************************************************
* Copyright (c) 2018, tkorays <tkorays@hotmail.com>
* All rights reserved.
*
* @file    path.h
* @brief   CFAST path helper.
* @version 1.0
* @author  tkorays<tkorays@hotmail.com>
* @date    2018-11-28
****************************************************/

#ifndef __CF_PATH_H__
#define __CF_PATH_H__

#include <cf/types.h>

#define CF_PATH_MAX_SIZE 256

#define CF_PATH_SPLIT_CHAR_WIN '\\'
#define CF_PATH_SPLIT_CHAR_UNI  '/'
#ifdef CF_OS_WIN
#   define CF_PATH_SPLIT_STR    "\\\\"
#   define CF_PATH_SPLIT_CHAR   '\\'
#else 
#   define CF_PATH_SPLIT_STR    "/"
#   define CF_PATH_SPLIT_CHAR   '/'
#endif

CF_DECLS_BEGIN

CF_DECLARE(cf_errno_t) cf_path_append(cf_char_t* path, cf_size_t size, const cf_char_t* p);
CF_DECLARE(cf_errno_t) cf_path_getcwd(cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_realpath(const cf_char_t* org, cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_basename(const cf_char_t* org, cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_dirname(const cf_char_t* org, cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_bool_t)  cf_path_exist(const cf_char_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isfile(const cf_char_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isdir(const cf_char_t* path);
CF_DECLARE(cf_bool_t)  cf_path_walk(const cf_char_t* path, cf_bool_t(*walk_func)(const cf_char_t*));

CF_DECLS_END


#endif /* __CF_PATH_H__ */
