#ifndef __CF_STREAM_H__
#define __CF_STREAM_H__

#include "cf/types.h"
#include "cf/memory.h"

/**
 * stream reader 
 */
typedef struct {
    cf_uint8_t*     data;
    cf_size_t       len;
    cf_size_t       pos;
    cf_bool_t       net;
} cf_sr_t;

typedef cf_sr_t cf_sw_t;


#define cf_sr_init(_strm, _data, _len, _net) do { \
    (_strm)->data = (_data);                      \
    (_strm)->len = (_len);                        \
    (_strm)->net = (_net);                        \
    (_strm)->pos = 0;                             \
} while(0)
#define cf_sr_deinit(_strm) ((void)0)
#define cf_sr_ptr(_strm) ((_strm)->data)
#define cf_sr_pos(_strm) ((_strm)->pos)
#define cf_sr_remain(_strm) ((_strm)->len - (_strm)->pos)

#define cf_sw_init(_strm, _data, _len, _net) cf_sr_init((_strm), (_data), (_len), (_net))
#define cf_sw_deinit(_strm) ((void)0)
#define cf_sw_ptr(_strm) ((_strm)->data)
#define cf_sw_pos(_strm) ((_strm)->pos)
#define cf_sw_remain(_strm) ((_strm)->len - (_strm)->pos)

CF_FORCE_INLINE cf_uint8_t cf_sr_read8(cf_sr_t* self) {
    cf_uint8_t ret = *CF_TYPE_CAST(cf_uint8_t*, self->data + self->pos);
    self->pos += 1;
    return ret;
}

CF_FORCE_INLINE cf_uint16_t cf_sr_read16(cf_sr_t* self) {
    cf_uint16_t ret = *CF_TYPE_CAST(cf_uint16_t*, self->data + self->pos);
    self->pos += 2;
    return self->net ? cf_hton_u16(ret) : ret;
}

CF_FORCE_INLINE cf_uint32_t cf_sr_read32(cf_sr_t* self) {
    cf_uint32_t ret = *CF_TYPE_CAST(cf_uint32_t*, self->data + self->pos);
    self->pos += 4;
    return self->net ? cf_hton_u32(ret) : ret;
}

CF_FORCE_INLINE cf_uint64_t cf_sr_read64(cf_sr_t* self) {
    cf_uint64_t ret = *CF_TYPE_CAST(cf_uint64_t*, self->data + self->pos);
    self->pos += 8;
    return self->net ? cf_hton_u64(ret) : ret;
}

CF_FORCE_INLINE void cf_sr_read(cf_sr_t* self, cf_uint8_t* data, cf_size_t len) {
    cf_memcpy_s(data, len, self->data + self->pos, cf_sw_remain(self));
    self->pos += len;
}

CF_FORCE_INLINE void cf_sr_add_pos(cf_sr_t* self, cf_size_t len) {
    self->pos += len;
}

CF_FORCE_INLINE void cf_sw_write8(cf_sr_t* self, cf_uint8_t data) {
    *CF_TYPE_CAST(cf_uint8_t*, self->data + self->pos) = data;
    self->pos += 1;
}

CF_FORCE_INLINE void cf_sw_write16(cf_sr_t* self, cf_uint16_t data) {
    *CF_TYPE_CAST(cf_uint16_t*, self->data + self->pos) = self->net ? cf_hton_u16(data) : data;
    self->pos += 2;
}

CF_FORCE_INLINE void cf_sw_write32(cf_sr_t* self, cf_uint32_t data) {
    *CF_TYPE_CAST(cf_uint32_t*, self->data + self->pos) = self->net ? cf_hton_u32(data) : data;;
    self->pos += 4;
}

CF_FORCE_INLINE void cf_sw_write64(cf_sr_t* self, cf_uint64_t data) {
    *CF_TYPE_CAST(cf_uint64_t*, self->data + self->pos) = self->net ? cf_hton_u64(data) : data;;
    self->pos += 8;
}

CF_FORCE_INLINE void cf_sw_write(cf_sr_t* self, cf_uint8_t* data, cf_size_t len) {
    cf_memcpy_s(self->data + self->pos, cf_sw_remain(self), data, len);
    self->pos += len;
}

#endif