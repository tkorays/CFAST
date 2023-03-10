#ifndef __CF_HASHTBL_H__
#define __CF_HASHTBL_H__

#include "cf/types.h"

CF_DECLS_BEGIN

#define CF_HASH_KEY_LEN_AUTO (-1)

typedef struct cf_hashtbl cf_hashtbl_t;
typedef struct cf_hashtbl_iterator cf_hashtbl_iterator_t;

cf_uint32_t cf_hash_calc(cf_uint32_t init_val, cf_void_t* key, cf_size_t len);

cf_hashtbl_t* cf_hashtbl_new(cf_size_t size);
cf_void_t cf_hashtbl_delete(cf_hashtbl_t* self);
cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, cf_uint32_t key);
cf_bool_t cf_hashtbl_set(cf_hashtbl_t* self, cf_uint32_t, cf_void_t* value);
cf_size_t cf_hashtbl_size(cf_hashtbl_t* self);



CF_DECLS_END


#endif /* __CF_HASHTBL_H__ */
