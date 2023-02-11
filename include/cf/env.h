/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    env.h
 * @brief   environment manipulating
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-12-07
 ****************************************************/
#ifndef __CF_ENV_H__
#define __CF_ENV_H__

#include <cf/types.h>
#include <cf/err.h>

/**
 * @defgroup CF_ENV env
 * env
 * @{
 */

CF_DECLS_BEGIN

cf_errno_t cf_env_get(const cf_char_t* name, cf_char_t* value, cf_size_t size);
cf_errno_t cf_env_set(const cf_char_t* name, const cf_char_t* value);
cf_errno_t cf_env_delete(const cf_char_t* name);
cf_errno_t cf_env_get_home(cf_char_t* path, cf_size_t size);

CF_DECLS_END

/** @} */

#endif /* __CF_ENV_H__ */
