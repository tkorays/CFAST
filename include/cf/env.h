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
 * This module provides functions for manipulating environment variables and retrieving the path of the user's home directory.
 * @{
 */



CF_DECLS_BEGIN

/**
 * @brief Get the value of an environment variable.
 * 
 * @param name The name of the environment variable.
 * @param value The buffer to store the value of the environment variable.
 * @param size The size of the buffer.
 * @return cf_errno_t CF_OK if successful, otherwise an error code.
 */
cf_errno_t cf_env_get(const cf_char_t* name, cf_char_t* value, cf_size_t size);

/**
 * @brief Set the value of an environment variable.
 * 
 * @param name The name of the environment variable.
 * @param value The value to set the environment variable to.
 * @return cf_errno_t CF_OK if successful, otherwise an error code.
 */
cf_errno_t cf_env_set(const cf_char_t* name, const cf_char_t* value);

/**
 * @brief Delete an environment variable.
 * 
 * @param name The name of the environment variable to delete.
 * @return cf_errno_t CF_OK if successful, otherwise an error code.
 */
cf_errno_t cf_env_delete(const cf_char_t* name);

/**
 * @brief Get the path of the user's home directory.
 * 
 * @param path The buffer to store the path of the user's home directory.
 * @param size The size of the buffer.
 * @return cf_errno_t CF_OK if successful, otherwise an error code.
 */
cf_errno_t cf_env_get_home(cf_char_t* path, cf_size_t size);



CF_DECLS_END

/** @} */

#endif /* __CF_ENV_H__ */
