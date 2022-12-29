#ifndef __CF_BYTES_H__
#define __CF_BYTES_H__

#include <cf/types.h>

/* read integers from big endian buffer */
#define cf_read_u16_from_be(buf) cf_ntoh_u16(*CF_TYPE_CAST(cf_uint16_t*, buf))
#define cf_read_u32_from_be(buf) cf_ntoh_u32(*CF_TYPE_CAST(cf_uint32_t*, buf))

/**
 * @brief byte reader.
 * 
 */
typedef struct {
    cf_uint8_t*     data;
    cf_size_t       len;
    cf_size_t       pos;
} cf_byte_reader_t;

/**
 * @brief byte writer.
 * 
 */
typedef struct {
    cf_uint8_t*     buffer;
    cf_size_t       capacity;
    cf_size_t       length;
} cf_byte_writer_t;

CF_DECLS_BEGIN

/**
 * @brief init a byte reder
 * 
 */
#define cf_byte_reader_init(p, _data, _len) do {\
    (p)->data = (_data);    \
    (p)->len = (_len);      \
    (p)->pos = 0;           \
} while(0)

/**
 * @brief deinit a byte reader
 * 
 */
#define cf_byte_reader_deinit(p) ((void)0)

/**
 * @brief get remaining bytes for this reader
 * 
 */
#define cf_byte_reader_remaining(p) ((p)->len - (p)->pos)

/**
 * @brief a macro template for generate `cf_byte_reader_get*` functions.
 * 
 */
#define __cf_byte_reader_get_template__(type_name, type_name_short)                 \
CF_FORCE_INLINE cf_bool_t cf_byte_reader_get_##type_name_short(cf_byte_reader_t* self,       \
    type_name* value) {                                                             \
    if (sizeof(type_name) <= (self->len - self->pos)) {                             \
        *value = *CF_TYPE_CAST(type_name*, self->data + self->pos);                 \
        self->pos += sizeof(type_name);                                             \
        return CF_TRUE;                                                             \
    }                                                                               \
    return CF_FALSE;                                                                \
}

cf_bool_t cf_byte_reader_get(cf_byte_reader_t* self, char* buf, cf_size_t buf_len, cf_size_t rbytes);

/* cf_byte_reader_get_u8 */
__cf_byte_reader_get_template__(cf_uint8_t, u8)

/* cf_byte_reader_get_i16 */
__cf_byte_reader_get_template__(cf_int8_t, i8)

/* cf_byte_reader_get_u16 */
__cf_byte_reader_get_template__(cf_uint16_t, u16)

/* cf_byte_reader_get_i16 */
__cf_byte_reader_get_template__(cf_int16_t, i16)

/* cf_byte_reader_get_u32 */
__cf_byte_reader_get_template__(cf_uint32_t, u32)

/* cf_byte_reader_get_i32 */
__cf_byte_reader_get_template__(cf_int32_t, i32)

/* cf_byte_reader_get_u64 */
__cf_byte_reader_get_template__(cf_uint64_t, u64)

/* cf_byte_reader_get_i64 */
__cf_byte_reader_get_template__(cf_int64_t, i64)

/* cf_byte_reader_get_float */
__cf_byte_reader_get_template__(float, float)

/* cf_byte_reader_get_double */
__cf_byte_reader_get_template__(double, double)


#define cf_byte_writer_init(p, buf, size) do {  \
    (p)->buffer = (buf);                        \
    (p)->capacity = (size);                     \
    (p)->length = 0;                            \
} while(0)

#define cf_byte_writer_deinit(p) ((void)0)

#define cf_byte_writer_data(p) ((p)->buffer)

#define cf_byte_writer_length(p) ((p)->length)

#define cf_byte_writer_capacity(p) ((p)->capacity)

cf_bool_t cf_byte_writer_put(cf_byte_writer_t* self, char* data, cf_size_t length);

#define __cf_byte_writer_put_template__(type_name, type_name_short)                                 \
CF_FORCE_INLINE cf_bool_t cf_byte_writer_put_##type_name_short(cf_byte_writer_t* self, type_name value) {    \
    if (self->length > self->capacity) return CF_FALSE;                                             \
    if (self->capacity - self->length < sizeof(type_name)) return CF_FALSE;                         \
    *CF_TYPE_CAST(type_name*, self->buffer + self->length) = value;                                 \
    self->length += sizeof(type_name);                                                              \
    return CF_TRUE; \
}

/* cf_byte_writer_put_u8 */
__cf_byte_writer_put_template__(cf_uint8_t, u8)

/* cf_byte_writer_put_i16 */
__cf_byte_writer_put_template__(cf_int8_t, i8)

/* cf_byte_writer_put_u16 */
__cf_byte_writer_put_template__(cf_uint16_t, u16)

/* cf_byte_reader_get_i16 */
__cf_byte_writer_put_template__(cf_int16_t, i16)

/* cf_byte_reader_get_u32 */
__cf_byte_writer_put_template__(cf_uint32_t, u32)

/* cf_byte_reader_get_i32 */
__cf_byte_writer_put_template__(cf_int32_t, i32)

/* cf_byte_writer_put_u64 */
__cf_byte_writer_put_template__(cf_uint64_t, u64)

/* cf_byte_writer_put_i64 */
__cf_byte_writer_put_template__(cf_int64_t, i64)

/* cf_byte_writer_put_float */
__cf_byte_writer_put_template__(float, float)

/* cf_byte_writer_put_double */
__cf_byte_writer_put_template__(double, double)

CF_DECLS_END

#endif /* __CF_BYTES_H__ */
