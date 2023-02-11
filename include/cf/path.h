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

/**
 * @defgroup CF_PATH path utils
 * system path utils
 * @{
 */

#define CF_PATH_MAX_SIZE 256

#define CF_PATH_SPLIT_CHAR_WIN '\\'
#define CF_PATH_SPLIT_CHAR_UNI  '/'

CF_DECLS_BEGIN

CF_DECLARE(cf_errno_t) cf_path_append(cf_char_t* path, cf_size_t size, const cf_char_t* p);
CF_DECLARE(cf_errno_t) cf_path_getcwd(cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_realpath(const cf_char_t* org, cf_char_t* out, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_basename(const cf_char_t* org, cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_errno_t) cf_path_dirname(const cf_char_t* org, cf_char_t* path, cf_size_t size);
CF_DECLARE(cf_bool_t)  cf_path_isabs(const cf_char_t* path);


CF_DECLS_END

/** @} */

#endif /* __CF_PATH_H__ */
