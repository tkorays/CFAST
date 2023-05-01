#include "cf/array.h"
#include "cf/assert.h"
#include "cf/memory.h"

cf_bool_t array_cmp(const cf_void_t* a, const cf_void_t* b) {
    if (a == CF_NULL_PTR || b == CF_NULL_PTR) {
        return CF_FALSE;
    }
    return *CF_TYPE_CAST(int*, a) == *CF_TYPE_CAST(int*, b);
}

void test_array_expand() {
    cf_array_t arr;
    int i;
    int elm = 1234;
    cf_array_init(&arr, sizeof(int), 5);

    for (i = 0; i < 5; i++) {
        cf_array_insert(&arr, CF_ARRAY_END_INDEX, &elm, sizeof(int));
    }
    cf_assert(cf_array_size(&arr) == 5);
    cf_assert(arr.capacity == 5);
    cf_array_insert(&arr, CF_ARRAY_END_INDEX, &elm, sizeof(int));
    cf_assert(arr.capacity == 10);

    cf_array_deinit(&arr);
}
void test_array_insert_front_back() {
    int items_front[] = {1, 2, 3, 4};
    int items_back[] = {5, 6, 7, 8};
    cf_array_t arr;
    cf_array_init(&arr, sizeof(int), 5);
    int i;

    for (i = 0; i < CF_ARRAY_SIZE(items_front); i++) {
        cf_array_insert(&arr, CF_ARRAY_BEGIN_INDEX, &items_front[i], sizeof(int));
    }
    for (i = 0; i < CF_ARRAY_SIZE(items_back); i++) {
        cf_array_insert(&arr, CF_ARRAY_END_INDEX, &items_back[i], sizeof(int));
    }

    for (i = 0; i < CF_ARRAY_SIZE(items_front); i++) {
        cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, i)) == items_front[CF_ARRAY_SIZE(items_front) - 1 - i]);
    }

    for (i = CF_ARRAY_SIZE(items_front); i < CF_ARRAY_SIZE(items_back) + CF_ARRAY_SIZE(items_front); i++) {
        cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, i)) == items_back[i - CF_ARRAY_SIZE(items_front)]);
    }
    cf_array_deinit(&arr);
}

void test_array_insert_to_position() {
    cf_array_t arr;
    int i;
    int elm = 1234, elm1 = 6789;
    cf_array_init(&arr, sizeof(int), 5);

    for (i = 0; i < 5; i++) {
        cf_array_insert(&arr, CF_ARRAY_END_INDEX, &elm, sizeof(int));
    }
    cf_assert(cf_array_size(&arr) == 5);
    cf_assert(arr.capacity == 5);
    cf_array_insert(&arr, 3, &elm1, sizeof(int));
    cf_assert(arr.capacity == 10);

    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 3)) == elm1);

    cf_array_set(&arr, 3, &elm, sizeof(int));
    cf_assert(*CF_TYPE_CAST(int*, cf_array_get(&arr, 3)) == elm);

    cf_array_deinit(&arr);
}

void test_array_find() {
    int items[] = {1, 2, 3, 4, 5, 6, 7};
    cf_array_t arr;
    cf_array_init(&arr, sizeof(int), 5);
    int i;
    for (i = 0; i < CF_ARRAY_SIZE(items); i++) {
        cf_array_insert(&arr, CF_ARRAY_END_INDEX, &items[i], sizeof(int));
    }

    for (i = 0; i < CF_ARRAY_SIZE(items); i++) {
        cf_assert(cf_array_find(&arr, &items[i], sizeof(int), array_cmp) == i);
    }
    cf_array_deinit(&arr);
}

int main(int argc, char* argv[]) {
    cf_memchk_init();
    test_array_expand();
    test_array_insert_front_back();
    test_array_insert_to_position();
    test_array_find();
    cf_assert(cf_memchk_deinit_and_summary());
    return 0;
}
