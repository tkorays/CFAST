#ifndef __ZRTC_OPTIONAL_H__
#define __ZRTC_OPTIONAL_H__

#include "cf/types.h"


typedef struct {
    cf_bool_t has_val;
    union {
        cf_int8_t       val_int8;
        cf_uint8_t      val_uint8;
        cf_int16_t      val_int16;
        cf_uint16_t     val_uint16;
        cf_int32_t      val_int32;
        cf_uint32_t     val_uint32;
        cf_int64_t      val_int64;
        cf_uint64_t     val_uint64;
        float           val_float;
        double          val_double;
        void*           val_ptr;
    } v;
} cf_optional_t;

#define cf_optional_reset(opt) ((opt).has_val = CF_FALSE)
#define cf_optional_set(opt, type, v) do { (opt).has_val = CF_TRUE; cf_optional_get(opt, type) = (v); } while(0)
#define cf_optional_get(opt, type) ((opt).v.val_##type)
#define cf_optional_has_val(opt) ((opt).has_val)

#endif /* __ZRTC_OPTIONAL_H__ */
