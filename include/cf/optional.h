#include "cf/types.h"


typedef struct {
    cf_bool_t has_val;
    union {
        cf_int8_t       int8_val;
        cf_uint8_t      uint8_val;
        cf_int16_t      int16_val;
        cf_uint16_t     uint16_val;
        cf_int32_t      int32_val;
        cf_uint32_t     uint32_val;
        cf_int64_t      int64_val;
        cf_uint64_t     uint64_val;
        float           float_val;
        double          double_val;
    } v;
} cf_optional_t;

#define cf_optional_reset(opt) ((opt).has_val = CF_FALSE)
#define cf_optional_set(opt, type, v) do { (opt).has_val = CF_TRUE; cf_optional_get((opt), type) = (v); } while(0)
#define cf_optional_get(opt, type) ((opt).v.##type##_val)

void a() {
    cf_optional_t opt;
    opt.has_val = CF_TRUE;
    opt.v.int32_val = 1234;
    cf_optional_reset(opt);
    cf_optional_set(opt, int32, 4444);
    cf_optional_get(opt, int32);
}