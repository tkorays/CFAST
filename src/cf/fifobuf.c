#include "cf/fifobuf.h"
#include "cf/memory.h"

#define FIFOBUF_META_SIZE sizeof(cf_uint16_t)

struct cf_fifobuf {
    cf_char_t*  first;      /** first byte of the whole buffer */
    cf_uint16_t size;       /** buffer size */
    cf_uint16_t used;        /** used buffer size */
    cf_bool_t   own;        /** own the buffer */
    cf_char_t*  pbegin;     /** point to begin of the occupied buffer */
    cf_char_t*  pend;       /** point to end of the occupied buffer */
};

cf_fifobuf_t* cf_fifobuf_new(cf_void_t* nullable_buffer, unsigned size) {
    cf_fifobuf_t* fifo = cf_malloc_z(sizeof(cf_fifobuf_t));
    if (nullable_buffer) {
        fifo->first = nullable_buffer;
        fifo->own = CF_FALSE;
    } else {
        fifo->first = cf_malloc(size);
        fifo->own = CF_TRUE;
    }
    fifo->size = size;
    fifo->used = 0;
    fifo->pbegin = fifo->first;
    fifo->pend = fifo->first;
    return fifo;
}

cf_void_t* cf_fifobuf_delete(cf_fifobuf_t* self) {
    cf_void_t* buffer = CF_NULL_PTR;

    if (!self) { return CF_NULL_PTR; }
    if (self->own && self->first) {
        cf_free(self->first);
    } else {
        buffer = self->first;
    }
    cf_free(self);
    return buffer;
}

cf_size_t cf_fifobuf_size(cf_fifobuf_t* self) {
    return CF_TYPE_CAST(cf_size_t, self->size);
}

cf_bool_t cf_fifobuf_read(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t* size) {
    cf_char_t* ptr = CF_TYPE_CAST(cf_char_t*, buf);
    cf_size_t i, j;
    cf_uint16_t meta = 0;

    if (self->used == 0) {
        size = 0;
        return CF_FALSE;
    }

    i = CF_TYPE_CAST(cf_uintptr_t, self->pbegin) - CF_TYPE_CAST(cf_uintptr_t, self->first);
    meta |= CF_TYPE_CAST(cf_uint8_t, self->first[i++]);
    meta |= CF_TYPE_CAST(cf_uint16_t, self->first[(i++) % self->size]) << 8;

    if (meta > (self->used - FIFOBUF_META_SIZE)) {
        /** FIFO is corruption */
        size = 0;
        return CF_FALSE;
    }

    *size = meta;
    for (j = 0; j < meta; j++) {
        ptr[j] = self->first[(i++) % self->size];
    }
    self->used -= (FIFOBUF_META_SIZE + meta);
    self->pbegin = &self->first[i];
    return CF_TRUE;
}

cf_bool_t cf_fifobuf_write(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t size) {
    cf_char_t* ptr = CF_TYPE_CAST(cf_char_t*, buf);
    cf_size_t i, j;
    cf_uint16_t meta = CF_TYPE_CAST(cf_uint16_t, size);

    if (CF_TYPE_CAST(cf_size_t, self->size - self->used) < size) {
        return CF_FALSE;
    }

    i = CF_TYPE_CAST(cf_uintptr_t, self->pend) - CF_TYPE_CAST(cf_uintptr_t, self->first);
    
    /* write meta data */
    self->first[i++] = meta & 0x00FF;
    self->first[(i++) % self->size] = meta >> 8;

    /* write data */
    for (j = 0; j < size; j++) {
        self->first[(i++) % self->size] = ptr[j];
    }
    self->pend = &self->first[i];
    self->used += CF_TYPE_CAST(cf_uint16_t, FIFOBUF_META_SIZE + size);

    return CF_TRUE;
}

cf_bool_t cf_fifobuf_peek(cf_fifobuf_t* self, cf_void_t* buf, cf_size_t* size) {
    cf_char_t* ptr = CF_TYPE_CAST(cf_char_t*, buf);
    cf_size_t i, j;
    cf_uint16_t meta = 0;

    if (self->used == 0) {
        size = 0;
        return CF_FALSE;
    }

    i = CF_TYPE_CAST(cf_uintptr_t, self->pbegin) - CF_TYPE_CAST(cf_uintptr_t, self->first);
    meta |= CF_TYPE_CAST(cf_uint8_t, self->first[i++]);
    meta |= CF_TYPE_CAST(cf_uint16_t, self->first[(i++) % self->size]) << 8;

    if (meta > (self->used - FIFOBUF_META_SIZE)) {
        /** FIFO is corruption */
        size = 0;
        return CF_FALSE;
    }

    *size = meta;
    for (j = 0; j < meta; j++) {
        ptr[j] = self->first[(i++) % self->size];
    }
    return CF_TRUE;
}
