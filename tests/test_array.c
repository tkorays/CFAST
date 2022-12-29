#include "cf/array.h"
#include "cf/assert.h"

cf_bool_t array_cmp(cf_void_t* a, cf_void_t* b) {
    if (a == CF_NULL_PTR || b == CF_NULL_PTR) {
        return CF_FALSE;
    }
    return *CF_TYPE_CAST(int*, a) == *CF_TYPE_CAST(int*, b);
}


void test_array_insert() {
    cf_array_t arr;
    int a;
    cf_array_init(&arr, sizeof(int), 5);

    a = 1234;
    cf_array_insert(&arr, 0, &a, sizeof(a));
    cf_assert(cf_array_size(&arr) == 1);

    a = 4567;
    cf_array_insert(&arr, 1, &a, sizeof(a));
    cf_assert(cf_array_size(&arr) == 2);

    a = 7890;
    cf_array_insert(&arr, CF_ARRAY_END_INDEX, &a, sizeof(a));
    cf_assert(cf_array_size(&arr) == 3);

    a = 9876;
    cf_array_insert(&arr, CF_ARRAY_BEGIN_INDEX, &a, sizeof(a));
    cf_assert(cf_array_size(&arr) == 4);


    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 0)) == 9876);
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 1)) == 1234);
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 2)) == 4567);
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 3)) == 7890);


    a = 1111;
    cf_array_set(&arr, 0, &a, sizeof(a));
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 0)) == a);

    a = 2222;
    cf_array_set(&arr, CF_ARRAY_END_INDEX, &a, sizeof(a));
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, CF_ARRAY_END_INDEX)) == a);

    a = 4567;
    cf_assert(cf_array_find(&arr, &a, sizeof(a), array_cmp) == 2);

    cf_array_reset(&arr);
    cf_assert(cf_array_size(&arr) == 0);
    cf_assert(cf_array_get(&arr, 0) == CF_NULL_PTR);

    cf_array_deinit(&arr);
}


int main(int argc, char* argv[]) {
    test_array_insert();
    return 0;
}
