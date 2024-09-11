#include "cf/map.h"
#include "cf/assert.h"
#include "cf/types.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    cf_map_t* m = cf_map_new(); 

    cf_map_set_u32(m, 100, (void*)100);
    cf_map_set_u32(m, 70, (void*)70);
    cf_map_set_u32(m, 150, (void*)150);
    cf_map_set_u32(m, 60, (void*)60);
    cf_map_set_u32(m, 200, (void*)200);

    cf_assert(cf_map_get_u32(m, 60) == (void*)60);
    cf_assert(cf_map_get_u32(m, 70) == (void*)70);
    cf_assert(cf_map_get_u32(m, 100) == (void*)100);
    cf_assert(cf_map_get_u32(m, 150) == (void*)150);
    cf_assert(cf_map_get_u32(m, 200) == (void*)200);

    cf_uintptr_t exp[] = {60, 70, 100, 150, 200};
    int i = 0;
    cf_map_iter_t it = cf_map_iter_init(m);
    while (!cf_map_iter_end(it)) {
        cf_assert(CF_TYPE_CAST(cf_uintptr_t, cf_map_iter_value(it)) == exp[i]);
        it = cf_map_iter_next(it);
        i++;
    }

    cf_map_delete(m);
    return 0;
}
