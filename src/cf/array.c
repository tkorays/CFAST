#include "cf/algorithm.h"
#include "cf/types.h"
#include <cf/array.h>
#include <cf/memory.h>
#include <cf/err.h>
#include <cf/assert.h>

#define cf_array_clear(arr) do {    \
    (arr)->capacity = 0;            \
    (arr)->elm_size = 0;            \
    (arr)->elm_count = 0;           \
    (arr)->elm_start = CF_NULL_PTR; } while(0)

cf_bool_t cf_array_init(cf_array_t* self, cf_size_t el_size, cf_int_t capacity) {
    if (el_size > 1024) {
        return CF_FALSE;;
    }
    cf_array_clear(self);

    self->elm_start = cf_malloc(el_size * capacity);
    if (CF_NULL_PTR == self->elm_start) {
        return CF_FALSE;
    }
    self->capacity  = capacity;
    self->elm_size  = el_size;
    self->elm_count = 0;
    
    return CF_TRUE;
}

cf_void_t cf_array_deinit(cf_array_t* self) {
    if (self->elm_start) {
        cf_free(self->elm_start);
    }
    cf_array_clear(self);
}

cf_bool_t cf_array_insert(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size) {
    int i;
    cf_size_t new_memory_size = 0;
    cf_void_t* new_memory = CF_NULL_PTR;
    
    cf_assert(size <= self->elm_size);

    // out of range
    if (index < CF_ARRAY_END_INDEX || index > CF_TYPE_CAST(int, self->elm_count)) {
        return CF_FALSE;
    }

    // expand
    if (self->elm_count == self->capacity) {
        new_memory_size = self->elm_size * self->capacity * 2;
        new_memory = cf_malloc(new_memory_size);
        if (new_memory == CF_NULL_PTR) {
            return CF_FALSE;
        }
        cf_memcpy_s(new_memory, new_memory_size, self->elm_start, self->elm_count * self->elm_size);

        cf_free(self->elm_start);
        self->elm_start = new_memory;
        self->capacity *= 2;
    }

    // append to last
    if (index == CF_ARRAY_END_INDEX) {
        cf_memcpy_s((cf_uint8_t*)self->elm_start + self->elm_size * self->elm_count,
            self->elm_size,
            elm,
            self->elm_size);
        self->elm_count += 1;
        return CF_TRUE;
    }

    i = index;
    for (i = self->elm_count; i > index; i--) {
        cf_memcpy_s((cf_uint8_t*)self->elm_start + self->elm_size * i,
            self->elm_size,
            (cf_uint8_t*)self->elm_start + self->elm_size * (i - 1),
            self->elm_size);
    }
    cf_memcpy_s((cf_uint8_t*)self->elm_start + self->elm_size * index,
            self->elm_size,
            elm,
            self->elm_size);
    self->elm_count += 1;

    return CF_TRUE;
}

cf_bool_t cf_array_erase(cf_array_t* self, int index) {
    int i;
    if (index < CF_ARRAY_END_INDEX || index >= self->elm_count || self->elm_count <= 0) {
        return CF_FALSE;
    }
    if (index == CF_ARRAY_END_INDEX) {
        self->elm_count -= 1;
        return CF_TRUE;
    }
    for (i = index; i < self->elm_count - 1; i++) {
        cf_memcpy_s((cf_uint8_t*)self->elm_start + self->elm_size * i,
            self->elm_size,
            (cf_uint8_t*)self->elm_start + self->elm_size * (i + 1),
            self->elm_size);
    }
    self->elm_count -= 1;
    return CF_TRUE;
}

cf_bool_t cf_array_set(cf_array_t* self, int index, cf_void_t* elm, cf_size_t size) {
    cf_assert(size <= self->elm_size);
    if (index < CF_ARRAY_END_INDEX || index >= CF_TYPE_CAST(int, self->elm_count) || self->elm_count <= 0) {
        return CF_FALSE;
    }
    if (index == CF_ARRAY_END_INDEX) {
        index = self->elm_count - 1;
    }
    cf_memcpy_s((cf_uint8_t*)self->elm_start + self->elm_size * index,
            self->elm_size,
            elm,
            self->elm_size);
    return CF_TRUE;
}

cf_void_t* cf_array_get(cf_array_t* self, int index) {
    if (index < CF_ARRAY_END_INDEX || index >= CF_TYPE_CAST(int, self->elm_count) || self->elm_count <= 0) {
        return CF_NULL_PTR;
    }
    if (index == CF_ARRAY_END_INDEX) {
        index = self->elm_count - 1;
    }
    return CF_TYPE_CAST(cf_void_t*, (cf_uint8_t*)self->elm_start + self->elm_size * index);
}

cf_int_t cf_array_find(cf_array_t* self, cf_void_t* elm, cf_size_t size, cf_alg_equal_f cmp) {
    int i;
    if (self->elm_count <= 0) {
        return CF_ARRAY_INVALID_INDEX;
    }
    for (i = 0; i < self->elm_count; i++) {
        if (cmp(elm, (cf_uint8_t*)self->elm_start + self->elm_size * i)) {
            return i;
        }
    }

    return CF_ARRAY_INVALID_INDEX;
}

cf_bool_t cf_array_reset(cf_array_t* self) {
    self->elm_count = 0;
    return CF_TRUE;
}

