#ifndef __CF_HASHTBL_H__
#define __CF_HASHTBL_H__

#include "cf/types.h"

CF_DECLS_BEGIN

/**
 * @defgroup CF_HASH hash
 *
 * This module provides a simple hash table implementation.
 *
 * @{
 */

#define CF_HASH_STRING_KEY_LEN CF_TYPE_CAST(cf_size_t, -1)

/**
 * @brief this is the type declaration for hash table.
 */
typedef struct cf_hashtbl cf_hashtbl_t;


/**
 * @brief create a new hash table instance with fixed table size.
 * 
 * @param   size    a fixed size for table capacity.
 * @return          created hash table instance.
 */
cf_hashtbl_t* cf_hashtbl_new(cf_size_t size);

/**
 * @brief destroy a hash table instance.
 *
 * @param   self    this pointer.
 */
cf_void_t cf_hashtbl_delete(cf_hashtbl_t* self);

/**
 * @brief get value from hash table with a uint32 hash key.
 *
 * @param   self    this pointer.
 * @param   hash    hash key in uint32 format.
 * @return          hash value for the key.
 */
cf_void_t* cf_hashtbl_get_by_hash(cf_hashtbl_t* self, cf_uint32_t hash);

/**
 * @brief set value with a uint32 hash key.
 *
 * @param   self    this pointer
 * @param   hash    hash key in uint32 format.
 * @param   value   value to be set.
 */
cf_void_t cf_hashtbl_set_by_hash(cf_hashtbl_t* self, cf_uint32_t hash, cf_void_t* value);

/**
 * get value from hash table with a common key.
 */
cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len);


/**
 * set value with a common key.
 */
cf_void_t cf_hashtbl_set(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len, cf_void_t* value);

/**
 * return the count of items in hashtable.
 */
cf_size_t cf_hashtbl_size(cf_hashtbl_t* self);

/**
 * @}
 */

CF_DECLS_END


#endif /* __CF_HASHTBL_H__ */
