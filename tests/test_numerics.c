#include "cf/numerics.h"
#include "cf/assert.h"

int main(int argc, char *argv[])
{
    cf_unwrapper_t unwrapper; 
    cf_unwrapper_init(&unwrapper);

    cf_assert(cf_unwrapper_u8(&unwrapper, 123) == 123);
    cf_assert(cf_unwrapper_u8(&unwrapper, 233) == 233);
    cf_assert(cf_unwrapper_u8(&unwrapper, 1) == 257);

    cf_unwrapper_reset(&unwrapper);
    cf_assert(cf_unwrapper_u8(&unwrapper, 0) == 0);
    cf_assert(cf_unwrapper_u8(&unwrapper, 254) == -2);
    return 0;
}

