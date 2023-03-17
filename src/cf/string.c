#include <cf/string.h>
#include <cf/memory.h>
#include <cf/assert.h>
#include <string.h>

cf_bool_t cf_string_init(cf_string_t* self, const cf_char_t* str, cf_size_t len) {
    if (len == 0) return CF_FALSE;

    self->ref       = CF_FALSE;
    self->ptr       = cf_malloc(len + 1);
    self->len       = len;

    cf_memcpy_s(self->ptr, self->len, str, len);
    self->ptr[len]  = '\0';
    return CF_TRUE;
}

void cf_string_deinit(cf_string_t* self) {
    if (self->ptr && !self->ref) {
        cf_free(self->ptr);
    }
    cf_string_clear(self);
}


int cf_string_cmp_raw(const cf_string_t* self, const char* s, cf_size_t s_len) {
    const char* ptr = self->ptr;
    cf_size_t len = self->len;
    for (; *ptr == *s && len > 0 && s_len > 0; ++ptr, ++s, --len, --s_len);
    if (len == 0 && s_len == 0) return 0;
    if (len > 0 && s_len == 0) return 1;
    if (s_len > 0 && len == 0) return -1;
    return (*CF_TYPE_CAST(cf_uchar_t*, ptr) < *CF_TYPE_CAST(cf_uchar_t*, s) ? -1 : 1);
}

void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len) {
    cf_string_deinit(self);
    self->ptr       = ptr;
    self->len       = len;
}
