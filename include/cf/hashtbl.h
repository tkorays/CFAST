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
 * hashtable iterator.
 * This struct is used to iterate through the items in the hashtable.
 * You can use the functions cf_hashtbl_iter_init, cf_hashtbl_iter_next, and cf_hashtbl_iter_end to iterate through the items.
 */
typedef struct hashtbl_node* cf_hashtbl_iter_t;


/**
 * This is a callback function that is used to destroy values in the hashtable.
 * You can pass this function as an argument to the cf_hashtbl_delete function to destroy all values in the hashtable.
 */
typedef cf_void_t(*cf_hashtbl_cb_f)(cf_void_t* value);

/**
 * @brief create a new hash table instance with fixed table size.
 * 
 * @param   size    a fixed size for table capacity.
 * @return          created hash table instance.
 */
cf_hashtbl_t* cf_hashtbl_new(cf_size_t size, cf_hashtbl_cb_f value_cb);

/**
 * @brief destroy a hash table instance.
 *
 * @param   self    this pointer.
 * @param   cb      callback for destroy value items
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
 * @param   value   value to be set, a null value means clear the item in hashtbl.
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
cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, const cf_void_t* key, cf_size_t len);


/**
 * @brief set value with a common key.
 *
 * see `cf_hashtbl_get` for some notes.
 *
 * @param   self    this pointer
 * @param   key     hash key value
 * @param   len     bytes of this key
 * @param   value   value to be set, a null value means clear the item in hashtbl
 */
cf_void_t cf_hashtbl_set(cf_hashtbl_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value);

/**
 * @brief return the count of items in hashtable.
 * 
 * @param   self    this pointer
 * @return          item count of this hashtable
 */
cf_size_t cf_hashtbl_size(cf_hashtbl_t* self);

/**
 * @brief initialize a hashtable iterator.
 * 
 * @param   self    this pointer.
 * @return          an iterator pointing to the first item in the hashtable.
 */
cf_hashtbl_iter_t cf_hashtbl_iter_init(cf_hashtbl_t* self);

/**
 * @brief move the iterator to the next item in the hashtable.
 * 
 * @param   self    this pointer.
 * @param   it      the current iterator.
 * @return          an iterator pointing to the next item in the hashtable.
 */
cf_hashtbl_iter_t cf_hashtbl_iter_next(cf_hashtbl_t* self, cf_hashtbl_iter_t it);

/**
 * @brief check if the iterator has reached the end of the hashtable.
 * 
 * @param   self    this pointer.
 * @param   it      the current iterator.
 * @return          CF_TRUE if the iterator has reached the end of the hashtable, CF_FALSE otherwise.
 */
CF_FORCE_INLINE cf_bool_t cf_hashtbl_iter_end(cf_hashtbl_t* self, cf_hashtbl_iter_t it) {
    return it == CF_NULL_PTR ? CF_TRUE : CF_FALSE;
}

/**
 * @brief get the hash value of the item pointed to by the iterator.
 * 
 * @param   it      the current iterator.
 * @return          the hash value of the item pointed to by the iterator.
 */
cf_uint32_t cf_hashtbl_iter_hash(cf_hashtbl_iter_t it);

/**
 * @brief get the key of the item pointed to by the iterator.
 * 
 * @param   it      the current iterator.
 * @return          the key of the item pointed to by the iterator.
 */
cf_void_t* cf_hashtbl_iter_key(cf_hashtbl_iter_t it);

/**
 * @brief get the value of the item pointed to by the iterator.
 * 
 * @param   it      the current iterator.
 * @return          the value of the item pointed to by the iterator.
 */
cf_void_t* cf_hashtbl_iter_value(cf_hashtbl_iter_t it);


/**
 * @}
 */

CF_DECLS_END


#endif /* __CF_HASHTBL_H__ */
