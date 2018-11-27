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
#ifdef CF_OS_WIN
#   define CF_PATH_SPLIT_STR    "\\\\"
#else 
#   define CF_PATH_SPLIT_STR    "/"
#endif

CF_DECLS_BEGIN

typedef struct cf_path {
    cf_char_t path[CF_PATH_MAX_SIZE];
    cf_size_t len;
} cf_path_t;

CF_DECLARE(cf_errno_t) cf_path_init(cf_path_t* path, const cf_char_t* p);
CF_DECLARE(cf_errno_t) cf_path_empty(cf_path_t* path);
CF_DECLARE(cf_errno_t) cf_path_append(cf_path_t* path, const cf_char_t* p);
CF_DECLARE(cf_errno_t) cf_path_rel(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_abs(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_basename(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_errno_t) cf_path_dirname(const cf_path_t* in_path, cf_path_t* out_path);
CF_DECLARE(cf_bool_t)  cf_path_exist(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isabs(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isfile(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_isdir(const cf_path_t* path);
CF_DECLARE(cf_bool_t)  cf_path_walk(const cf_path_t* path, cf_bool_t(*walk_func)(const cf_path_t*));

CF_DECLS_END


#endif /* __CF_PATH_H__ */
