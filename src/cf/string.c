#include <cf/string.h>
#include <cf/memory.h>
#include <cf/assert.h>
#include <string.h>

cf_bool_t cf_string_init(cf_string_t* self, cf_size_t len, const cf_char_t* str) {
    if (len == 0) return CF_FALSE;

    cf_string_clear(self);

    self->allocator = cf_allocator_get(CF_ALLOCATOR_ID_DEFAULT);
    self->ptr       = self->allocator->new_fn(self->allocator, len + 1);
    self->len       = len;

    if (str) {
        cf_memcpy_s(self->ptr, self->len, str, len);
    }
    self->ptr[len]  = '\0';

    return CF_TRUE;
}

cf_bool_t cf_string_init_with_allocator(cf_string_t* self, cf_size_t len, const cf_char_t* str, cf_allocator_t* alloc) {
    if (len == 0 || !alloc) return CF_FALSE;

    cf_string_clear(self);

    self->allocator = alloc;
    self->ptr       = self->allocator->new_fn(self->allocator, len + 1);
    self->len       = len;

    if (str) {
        cf_memcpy_s(self->ptr, self->len, str, len);
    }
    self->ptr[len]  = '\0';

    return CF_TRUE;
}

void cf_string_deinit(cf_string_t* self) {
    if (self->allocator && self->ptr) {
        self->allocator->delete_fn(self->allocator, self->ptr);
    }

    cf_string_clear(self);
}

int cf_string_cmp_raw(cf_string_t* self, const char* s, cf_size_t s_len) {
    const char* ptr = self->ptr;
    cf_size_t len = self->len;
    for (; *ptr == *s && len > 0 && s_len > 0; ++ptr, ++s, --len, --s_len);
    if (len == 0 && s_len == 0) return 0;
    if (len > 0 && s_len == 0) return 1;
    if (s_len > 0 && len == 0) return -1;
    return (*CF_TYPE_CAST(cf_uchar_t*, ptr) < *CF_TYPE_CAST(cf_uchar_t*, s) ? -1 : 1);
}

void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len, cf_allocator_t* alloc) {
    cf_string_deinit(self);
    self->ptr       = ptr;
    self->len       = len;
    self->allocator = alloc;
}
