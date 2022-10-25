#include "cf/types.h"
#include "cf/assert.h"


int main(int argc, char const *argv[]) {
    cf_assert(sizeof(cf_int16_t) == 2);
    cf_assert(sizeof(cf_int32_t) == 4);
    cf_assert(sizeof(cf_int64_t) == 8);
    cf_assert(sizeof(cf_uintptr_t) == sizeof(uintptr_t));
    return 0;
}
