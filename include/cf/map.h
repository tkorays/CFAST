#ifndef __CF_MAP_H__
#define __CF_MAP_H__

#include "cf/types.h"
#include "cf/algorithm.h"
#include "cf/value.h"

typedef struct cf_map cf_map_t;
typedef void* cf_map_iter_t;

// cf_alg_cmp_f
cf_map_t* cf_map_new();
void cf_map_delete(cf_map_t* self);
cf_size_t cf_map_size(cf_map_t* self);

void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len);
void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value);

void* cf_map_get_u32(cf_map_t* self, cf_uint32_t key);
void cf_map_set_u32(cf_map_t* self, cf_uint32_t key, cf_void_t* value);

cf_map_iter_t cf_map_iter_init(cf_map_t* self);
cf_map_iter_t cf_map_iter_next(cf_map_iter_t it);
cf_bool_t cf_map_iter_end(cf_map_iter_t it);
void* cf_map_iter_value(cf_map_iter_t it);

void* __cf_map_get_internal(cf_map_t* self, cf_value_t* map_key);
void __cf_map_set_internal(cf_map_t* self, cf_value_t* map_key, cf_void_t* value);
#endif /** __CF_MAP_H__ */

