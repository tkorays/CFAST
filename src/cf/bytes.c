#include "cf/bytes.h"
#include "cf/memory.h"

cf_bool_t cf_byte_reader_get_(cf_byte_reader_t* self, char* buf, cf_size_t buf_len, cf_size_t rbytes) {
    if (rbytes <= (self->len - self->pos)) {
        if (CF_NULL_PTR == cf_memcpy_s(buf, buf_len, self->data + self->pos, self->len - self->pos)) {
            return CF_FALSE;
        }
        self->pos += rbytes;
        return CF_TRUE;
    }
    return CF_FALSE;
}

cf_bool_t cf_byte_writer_put(cf_byte_writer_t* self, char* data, cf_size_t length) {
    if (self->length > self->capacity) return CF_FALSE;
    if (CF_NULL_PTR == cf_memcpy_s(self->buffer + self->length, self->capacity - self->length, data, length)) {
        return CF_FALSE;
    }
    self->length += length;
    return CF_TRUE;
}
