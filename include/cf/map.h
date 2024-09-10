#ifndef __CF_MAP_H__
#define __CF_MAP_H__

#include "cf/types.h"
#include "cf/algorithm.h"

typedef struct cf_map cf_map_t;
typedef struct cf_map_iter cf_map_iter_t;

// cf_alg_cmp_f
cf_map_t* cf_map_new();
void cf_map_delete(cf_map_t* self);
cf_size_t cf_map_size(cf_map_t* self);

void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len);
void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value);

cf_map_iter_t cf_map_iter_init(cf_map_t* self);
cf_map_iter_t cf_map_iter_next(cf_map_iter_t it);
cf_bool_t cf_map_iter_end(cf_map_iter_t it);
void* cf_map_iter_value(cf_map_iter_t it);

#endif /** __CF_MAP_H__ */

