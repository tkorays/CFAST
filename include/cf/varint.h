#ifndef __CF_VARINT_H__
#define __CF_VARINT_H__

#include "cf/types.h"


typedef struct {
    cf_uint8_t  data[8];
    cf_uint8_t  bytes; 
} cf_varint_t;

void cf_varint_init(cf_varint_t* var);
void cf_varint_reset(cf_varint_t* var);
void cf_varint_from_u8(cf_varint_t* var, cf_uint8_t i);

cf_uint8_t cf_varint_bytes(const cf_varint_t* var);
cf_int64_t cf_varint_to_i64(const cf_varint_t* var);
cf_uint64_t cf_varint_to_u64(const cf_varint_t* var);


#endif /* __CF_VARINT_H__ */

