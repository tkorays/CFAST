#include "cf/numerics.h"


void cf_unwrapper_init(cf_unwrapper_t* self) {
    self->last_unwrapped = 0; 
    self->has_value = CF_FALSE;
    self->last_value = 0;
}

void cf_unwrapper_reset(cf_unwrapper_t* self) {
    cf_unwrapper_init(self);
}

cf_int64_t cf_unwrapper_u8(cf_unwrapper_t* self, cf_uint8_t n) {
    cf_int64_t WRAP = 0;
    if (!self->has_value) {
        self->last_unwrapped = n;
    } else {
        WRAP = CF_TYPE_CAST(cf_int64_t, 0x00FF) + 1;
        self->last_unwrapped += (
            self->last_value <= n ?
                n - self->last_value :
                WRAP - (self->last_value - n)
        );
        // handle going back
        if (n > self->last_value && n - self->last_value > WRAP / 2) {
            self->last_unwrapped -= WRAP;
        }
    }
    self->has_value = CF_TRUE;
    self->last_value = n;
    return self->last_unwrapped;
}

cf_int64_t cf_unwrapper_u16(cf_unwrapper_t* self, cf_uint16_t n) {
    cf_int64_t WRAP = 0;
    if (!self->has_value) {
        self->last_unwrapped = n;
    } else {
        WRAP = CF_TYPE_CAST(cf_int64_t, 0x00FFFF) + 1;
        self->last_unwrapped += (
            self->last_value <= n ?
                n - self->last_value :
                WRAP - (self->last_value - n)
        );
        // handle going back
        if (n > self->last_value && n - self->last_value > WRAP / 2) {
            self->last_unwrapped -= WRAP;
        }
    }
    self->has_value = CF_TRUE;
    self->last_value = n;
    return self->last_unwrapped;
}

cf_int64_t cf_unwrapper_u32(cf_unwrapper_t* self, cf_uint32_t n) {
    cf_int64_t WRAP = 0;
    if (!self->has_value) {
        self->last_unwrapped = n;
    } else {
        WRAP = CF_TYPE_CAST(cf_int64_t, 0x00FFFFFFFF) + 1;
        self->last_unwrapped += (
            self->last_value <= n ?
                n - self->last_value :
                WRAP - (self->last_value - n)
        );
        // handle going back
        if (n > self->last_value && n - self->last_value > WRAP / 2) {
            self->last_unwrapped -= WRAP;
        }
    }
    self->has_value = CF_TRUE;
    self->last_value = n;
    return self->last_unwrapped;
}

