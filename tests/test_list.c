#include "cf/list.h"
#include "cf/assert.h"
#include <stdio.h>


void test_list() {
    cf_list_t li;
    int a = 1, b = 2, c = 3, d = 4;
    cf_list_iter_t iter;
    cf_list_init(&li);
    cf_list_insert(&li, 0, &a);
    cf_assert(cf_list_size(&li) == 1);

    cf_list_insert(&li, CF_LIST_POS_TAIL, &b);
    cf_assert(cf_list_size(&li) == 2);

    cf_list_insert(&li, CF_LIST_POS_TAIL, &c);
    cf_assert(cf_list_size(&li) == 3);

    cf_list_insert(&li, CF_LIST_POS_TAIL, &d);
    cf_assert(cf_list_size(&li) == 4);


    cf_assert(*CF_TYPE_CAST(int*, cf_list_head(&li)) == a);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_tail(&li)) == d);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_get(&li, 1)) == b);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_get(&li, CF_LIST_POS_TAIL)) == d);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_erase(&li, 1)) == b);
    cf_assert(cf_list_size(&li) == 3);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_erase(&li, CF_LIST_POS_TAIL)) == d);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_tail(&li)) == c);

    for (iter = cf_list_iter_init(&li); iter; iter = cf_list_iter_next(iter)) {
        printf("data:%d\n", *CF_TYPE_CAST(int*, cf_list_iter_data(iter)));
    }

    cf_list_deinit(&li);
}


int main(int argc, char* argv[]) {
    test_list();
    return 0;
}
