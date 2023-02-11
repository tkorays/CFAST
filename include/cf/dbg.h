/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    dbg.h
 * @brief   Debug utils.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-2-19
 ****************************************************/
#ifndef __CF_DBG_H__
#define __CF_DBG_H__

#include <cf/types.h>

/**
 * @defgroup CF_DBG debug
 * Debug utils
 * @{
 */

CF_DECLS_BEGIN

/**
 * Dump memory as hex format.
 * @param m         Memory address.
 * @param n         How many bytes to dump.
 * @param buf       Buffer to save hex string.
 * @param size      Size of buffer.
 * @return          Out size.
 */
CF_DECLARE(cf_size_t) cf_dbg_mem2hex(cf_void_t* m, cf_size_t n, cf_char_t* buf, cf_size_t size);

CF_DECLS_END

/** @} */

#endif /* __CF_DBG_H__ */
