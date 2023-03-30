/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    dso.h
 * @brief   Dynamic Object Handling.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-6
 ****************************************************/
#ifndef __CF_DSO_H__
#define __CF_DSO_H__

#include <cf/types.h>

/**
 * @defgroup CF_DSO DSO
 * Dynamic Shared Object
 * @{
 */

CF_DECLS_BEGIN

/**
 * DSO handle.
 */
typedef struct cf_dso_handle cf_dso_handle_t;

/**
 * Symbol of a DSO.
 */
typedef cf_void_t* cf_dso_sym_t;

/**
 * Load a DSO library.
 *
 * @param handle Pointer to a DSO handle.
 * @param path Path to the DSO library.
 *
 * @return CF_EOK on success, otherwise an error code.
 */
CF_DECLARE(cf_errno_t) cf_dso_load(cf_dso_handle_t** handle, const cf_char_t* path);

/**
 * Unload a DSO library.
 *
 * @param handle Pointer to a DSO handle.
 *
 * @return CF_EOK on success, otherwise an error code.
 */
CF_DECLARE(cf_errno_t) cf_dso_unload(cf_dso_handle_t* handle);

/**
 * Get a symbol fom DSO.
 *
 * @param handle Pointer to a DSO handle.
 * @param name Name of the symbol.
 * @param symbol Pointer to a symbol.
 *
 * @return CF_EOK on success, otherwise an error code.
 */
CF_DECLARE(cf_errno_t) cf_dso_sym(cf_dso_handle_t* handle, const cf_char_t* name, cf_dso_sym_t* symbol);

/**
 * Inject a dll to target process.
 *
 * @param pid Process ID of the target process.
 * @param path Path to the DSO library.
 *
 * @return CF_EOK on success, otherwise an error code.
 */
CF_DECLARE(cf_errno_t) cf_dso_inject(cf_uint32_t pid, const cf_char_t* path);

CF_DECLS_END

/** @} */

#endif /* __CF_DSO_H__ */
