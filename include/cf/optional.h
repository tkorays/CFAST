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
    } v;
} cf_optional_t;

#define cf_optional_reset(opt) ((opt).has_val = CF_FALSE)
#define cf_optional_set(opt, type, v) do { (opt).has_val = CF_TRUE; cf_optional_get(opt, type) = (v); } while(0)
#define cf_optional_get(opt, type) ((opt).v.val_##type)

void a() {
    cf_optional_t opt;
    opt.has_val = CF_TRUE;
    opt.v.val_int32 = 1234;
    cf_optional_reset(opt);
    cf_optional_set(opt, int32, 4444);
    cf_optional_get(opt, int32);
}