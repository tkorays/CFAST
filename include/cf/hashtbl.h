#ifndef __CF_HASHTBL_H__
#define __CF_HASHTBL_H__

#include "cf/types.h"

CF_DECLS_BEGIN

/**
 * @defgroup CF_HASH hash
 *
 * This module provides a simple hash table implementation. It includes functions to create and destroy a hash table instance, 
 * get and set values with a uint32 hash key or a common key, and return the count of items in the hashtable. 
 *
 * @{
 */

/**
 * when we pass -1 for a string length, it means that this string ends with `\0`,
 * and we can determin the string length automaticlly.
 */
#define CF_HASH_STRING_KEY_LEN_AUTO CF_TYPE_CAST(cf_size_t, -1)

/**
 * @brief this is the type declaration for hash table.
 * 
 * This module provides a simple hash table implementation. It includes functions to create and destroy a hash table instance, 
 * get and set values with a uint32 hash key or a common key, and return the count of items in the hashtable. 
 * 
 * @{
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
 * uint32 hash is used when the key type can be converted to uint32 type.
 * don't use this function when hash key is string or other types.
 *
 * @param   self    this pointer.
 * @param   hash    hash key in uint32 format.
 * @return          hash value for the key.
 */
cf_void_t* cf_hashtbl_get_by_hash(cf_hashtbl_t* self, cf_uint32_t hash);

/**
 * @brief set value with a uint32 hash key.
 *
 * like `cf_hashtbl_get_by_hash`, only use this function when key can be converted to uint32 type.
 *
 * @param   self    this pointer
 * @param   hash    hash key in uint32 format.
 * @param   value   value to be set.
 */
cf_void_t cf_hashtbl_set_by_hash(cf_hashtbl_t* self, cf_uint32_t hash, cf_void_t* value);

/**
 * @brief get value from hash table with a common key.
 *
 * we will calculate a uint32 hash from the key memory.
 * if you pass a string key, `len = CF_HASH_STRING_KEY_LEN_AUTO` means we automaticlly determin the string length.
 *
 * @param   self    this pointer
 * @param   key     key of the hash table, can be a common memory or a string memory.
 * @param   len     bytes of key
 * @return          hash value for this key
 */
cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len);


/**
 * @brief set value with a common key.
 *
 * see `cf_hashtbl_get` for some notes.
 *
 * @param   self    this pointer
 * @param   key     hash key value
 * @param   len     bytes of this key
 * @param   value   value to be set
 */
cf_void_t cf_hashtbl_set(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len, cf_void_t* value);

/**
 * @brief return the count of items in hashtable.
 * 
 * @param   self    this pointer
 * @return          item count of this hashtable
 */
cf_size_t cf_hashtbl_size(cf_hashtbl_t* self);

/**
 * @}
 */

CF_DECLS_END


#endif /* __CF_HASHTBL_H__ */
