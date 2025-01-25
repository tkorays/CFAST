#ifndef __CF_MAP_H__
#define __CF_MAP_H__

#include "cf/config.h"
#include "cf/types.h"
#include "cf/algorithm.h"
#include "cf/value.h"

/**
 * @brief map implemented by red-black tree.
 */
typedef struct cf_map cf_map_t;

/**
 * map interator
 */
typedef void* cf_map_iter_t;


/**
 * create a rbtree map instance
 * 
 * @param f map key compare function
 * @return map instance
 */
cf_map_t* cf_map_new(cf_value_cmp_f f);

/**
 * @brief destroy a map
 *
 * @param self  this pointer
 * @return none
 */
void cf_map_delete(cf_map_t* self);

/**
 * @breif get map item size
 *
 * @param self  this pointer
 * @return map size
 */
cf_size_t cf_map_size(cf_map_t* self);

/**
 * @brief get map item with key 
 *
 * @param   self    this pointer
 * @param   key     map key pointer
 * @param   len     key length
 * @return  map item address
 */
void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len);

/**
 * @brief set map item with key 
 *
 * @param   self    this pointer
 * @param   key     map key pointer
 * @param   len     key length
 * @param   value   value address
 */
void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value);

/**
 * initialize a map interator
 *
 * @param   self    map pointer
 * @return  iterator
 */
cf_map_iter_t cf_map_iter_init(cf_map_t* self);

/**
 * get next iterator
 *
 * @param   self    current iterator
 * @return next iterator
 */
cf_map_iter_t cf_map_iter_next(cf_map_iter_t it);

/**
 * iterator is end?
 * 
 * @param   it  current iterator
 * @return true if iterator not exist
 */
cf_bool_t cf_map_iter_end(cf_map_iter_t it);

/**
 * get iterator value
 *
 * @param   it iterator
 * @return  iterator value
 */
void* cf_map_iter_value(cf_map_iter_t it);

void* __cf_map_get_internal(cf_map_t* self, cf_value_t* map_key);

void __cf_map_set_internal(cf_map_t* self, cf_value_t* map_key, cf_void_t* value);

/**
 * macro for defining map get and set functions
 */
#define __DEFINE_MAP_GET_SET_FUNC__(type_short, type) CF_FORCE_INLINE void* \
    cf_map_get_##type_short(cf_map_t* self, type key) {\
        cf_value_t map_key; \
        cf_value_init(&map_key); \
        cf_value_set_##type_short(&map_key, key); \
        return __cf_map_get_internal(self, &map_key); \
    } \
    CF_FORCE_INLINE void cf_map_set_##type_short(cf_map_t* self, type key, cf_void_t* value) { \
        cf_value_t map_key; \
        cf_value_init(&map_key); \
        cf_value_set_##type_short(&map_key, key); \
        __cf_map_set_internal(self, &map_key, value); \
    }

__DEFINE_MAP_GET_SET_FUNC__(i8, cf_int8_t)
__DEFINE_MAP_GET_SET_FUNC__(u8, cf_uint8_t)
__DEFINE_MAP_GET_SET_FUNC__(i16, cf_int16_t)
__DEFINE_MAP_GET_SET_FUNC__(u16, cf_uint16_t)
__DEFINE_MAP_GET_SET_FUNC__(i32, cf_int32_t)
__DEFINE_MAP_GET_SET_FUNC__(u32, cf_uint32_t)
__DEFINE_MAP_GET_SET_FUNC__(i64, cf_int64_t)
__DEFINE_MAP_GET_SET_FUNC__(u64, cf_uint64_t)

#endif /** __CF_MAP_H__ */

