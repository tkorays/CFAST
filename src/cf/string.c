#include <cf/string.h>
#include <cf/memory.h>
#include <cf/assert.h>
#include <string.h>

cf_bool_t cf_string_init(cf_string_t* self, cf_size_t len) {
    if (len == 0) return CF_FALSE;

    cf_string_clear(self);

    self->ptr = cf_malloc(len);
    self->len = len;
    self->ref = CF_FALSE;

    return CF_TRUE;
}

void cf_string_deinit(cf_string_t* self) {
    if (!self->ref && self->ptr) {
        cf_free(self->ptr);
    }

    cf_string_clear(self);
}

void cf_string_reset(cf_string_t* self, cf_char_t* ptr, cf_size_t len, cf_bool_t ref) {
    cf_string_deinit(self);
    self->ptr = ptr;
    self->len = len;
    self->ref = ref;
}