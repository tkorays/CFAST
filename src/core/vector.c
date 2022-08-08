#include "cf/vector.h"
#include "cf/memory.h"


#define cf_vector_BUF_DEF_BYTES 512
#define cf_vector_BUF_DEF_COUNT 128

static inline cf_bool_t _cf_vectory_copy_buffer(cf_vector_t* vec, void* new_buffer, size_t new_capacity) {
    cf_uint64_t tmp1        = 0;
    cf_uint64_t tmp2        = 0;
    if (vec->elm_count == 0) {
        rtc_free(vec->buffer);
        vec->capacity  = new_capacity;
        vec->buffer    = new_buffer;
        vec->front     = new_buffer;
        vec->back      = new_buffer;
    } else {
        if ((cf_uint64_t)(vec->front) < (cf_uint64_t)(vec->back)) {
            memcpy(new_buffer, vec->front, vec->elm_count * vec->elm_size);
        } else {
            tmp1 = vec->capacity * vec->elm_size - ((cf_uint64_t)vec->front - (cf_uint64_t)vec->buffer + vec->elm_size);
            memcpy(new_buffer, vec->front, tmp1);
            tmp2 = vec->elm_count * vec->elm_size - tmp1;
            if (tmp2 > 0) {
                memcpy((cf_uint8_t*)new_buffer + tmp1, vec->buffer, tmp2);
            }
        }  
        rtc_free(vec->buffer);
        vec->capacity  = new_capacity;
        vec->buffer    = new_buffer;
        vec->front     = new_buffer;
        vec->back      = (void*)((cf_int8_t*)new_buffer + vec->elm_count * vec->elm_size);
    }

    return CF_TRUE;
}

static inline void* _cf_vector_next_of(void* buffer, size_t elm_size, size_t capacity, void* current) {
    int offset;
    offset = (cf_uint64_t)current - (cf_uint64_t)buffer;
    offset = offset == (capacity - 1) * elm_size ? 0 : offset + elm_size;
    return CF_TYPE_CAST(void*, (cf_uint8_t*)buffer + offset);
}

static inline void* _cf_vector_prev_of(void* buffer, size_t elm_size, size_t capacity, void* current) {
    int offset;
    offset = (cf_uint64_t)current - (cf_uint64_t)buffer;
    offset = offset == 0 ? (capacity - 1) * elm_size : offset - elm_size;
    return CF_TYPE_CAST(void*, (cf_uint8_t*)buffer + offset);
}

cf_bool_t cf_vector_init(cf_vector_t* self, size_t elm_size) {
    self->capacity  = CF_MIN2(cf_vector_BUF_DEF_BYTES, cf_vector_BUF_DEF_COUNT * elm_size) / elm_size;
    self->elm_size  = elm_size;
    self->buffer    = cf_malloc(self->capacity * elm_size);
    if (!self->buffer) {
        return CF_NULL_PTR;
    }
    self->elm_count = 0;
    self->front     = self->buffer;
    self->back      = self->buffer;
    return self;
}

void cf_vector_deinit(cf_vector_t* self) {
    if (self->buffer) {
        cf_free(self->buffer);
    }
    self->buffer    = CF_NULL_PTR;
    self->capacity  = 0;
    self->elm_size  = 0;
    self->elm_count = 0;
    self->front     = 0;
    self->back      = 0;
}

void cf_vector_reserve(cf_vector_t* vec, size_t cnt) {
    void* new_buffer    = CF_NULL_PTR;
    if (cnt <= vec->capacity) return;
    new_buffer = cf_malloc(cnt * vec->elm_size);
    if (!new_buffer) {
        return;
    }

    _cf_vectory_copy_buffer(vec, new_buffer, cnt);
}

cf_bool_t cf_vector_push_back(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;

    if (vec->capacity <= vec->elm_count) {
        cf_vector_reserve(vec, vec->capacity * 2);
        if (vec->capacity <= vec->elm_count) {
            return CF_FALSE;
        }
    }

    vec->back = vec->elm_count == 0 ? vec->buffer :
        _cf_vector_next_of(vec->buffer, vec->elm_size, vec->capacity, vec->back);
    memcpy(vec->back, data, size);
    vec->elm_count++;
    return CF_TRUE;
}

cf_bool_t cf_vector_push_front(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;

    if (vec->capacity <= vec->elm_count) {
        cf_vector_reserve(vec, vec->capacity * 2);
        if (vec->capacity <= vec->elm_count) {
            return CF_FALSE;
        }
    }
    vec->front = vec->elm_count == 0 ? vec->buffer :
        _cf_vector_prev_of(vec->buffer, vec->elm_size, vec->capacity, vec->front);
    memcpy(vec->front, data, size);
    vec->elm_count++;
    return CF_TRUE;
}

cf_bool_t cf_vector_pop_back(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;

    if (cf_vector_is_empty(vec)) return CF_FALSE;
    memcpy(data, vec->back, size);

    vec->back = _cf_vector_prev_of(vec->buffer, vec->elm_size, vec->capacity, vec->back);
    vec->elm_count--;
    return CF_TRUE;
}

cf_bool_t cf_vector_pop_front(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;

    if (cf_vector_is_empty(vec)) return CF_FALSE;
    memcpy(data, vec->front, size);
    vec->front = _cf_vector_next_of(vec->buffer, vec->elm_count, vec->capacity, vec->front);
    vec->elm_count--;
    return CF_TRUE;
}

cf_bool_t cf_vector_back(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;
    memcpy(data, vec->back, size);
    return CF_TRUE;
}

cf_bool_t cf_vector_front(cf_vector_t* vec, void* data, size_t size) {
    CF_CHECK_GE(size, vec->elm_size);
    if (!data) return CF_FALSE;
    memcpy(data, vec->front, size);
    return CF_TRUE;
}

cf_bool_t cf_vector_at(cf_vector_t* vec, int idx, void* data, size_t size) {
    if (!data || size != vec->elm_size) return CF_FALSE;
    memcpy(data, (cf_uint8_t*)vec->buffer, size);
    return CF_TRUE;
}

cf_bool_t cf_vector_resize(cf_vector_t* vec, size_t new_size, void* data) {
    if (new_size > vec->capacity) {
        cf_vector_reserve(vec, new_size);
        if (new_size > vec->capacity) return CF_FALSE;
    }

    while (vec->elm_count < new_size) {
        cf_vector_push_back(vec, data, vec->elm_size);
    }
    return CF_TRUE;
}

void cf_vector_clear(cf_vector_t* vec) {
    vec->elm_count = 0;
    vec->front     = vec->buffer;
    vec->back      = vec->buffer;
}

