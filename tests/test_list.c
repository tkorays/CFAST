#include "cf/list.h"
#include "cf/assert.h"
#include <stdio.h>

cf_bool_t li_item_cmp(const cf_void_t* a, const cf_void_t* b) {
    return *CF_TYPE_CAST(int*, a) == *CF_TYPE_CAST(int*, b);
}

void test_list() {
    cf_list_t li;
    int a = 1, b = 2, c = 3, d = 4;
    cf_list_iter_t iter;
    cf_list_iter_t it;

    cf_list_init(&li);

    cf_list_push_back(&li, &a);
    cf_assert(cf_list_size(&li) == 1);

    cf_list_push_back(&li, &b);
    cf_assert(cf_list_size(&li) == 2);

    cf_list_push_back(&li, &c);
    cf_assert(cf_list_size(&li) == 3);

    cf_list_push_back(&li, &d);
    cf_assert(cf_list_size(&li) == 4);

    cf_assert(*CF_TYPE_CAST(int*, cf_list_head(&li)) == a);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_tail(&li)) == d);

    iter = cf_list_find(&li, &b, li_item_cmp);
    cf_assert(*CF_TYPE_CAST(int*, cf_list_iter_data(iter)) == b);
    cf_list_erase(&li, iter);
    cf_assert(cf_list_size(&li) == 3);

    cf_list_erase(&li, cf_list_find(&li, &d, li_item_cmp));
    cf_list_erase(&li, cf_list_find(&li, &a, li_item_cmp));

    cf_assert(*CF_TYPE_CAST(int*, cf_list_tail(&li)) == c);

    it = cf_list_iter_init(&li);
    while (!cf_list_iter_end(&li, it)) {
        printf("%d\n", CF_P2V_CAST(int*, it->data));
        it = cf_list_iter_next(it);
    }

    iter = cf_list_iter_init(&li);
    cf_list_erase(&li, iter);
    cf_assert(cf_list_size(&li) == 0);
    cf_assert(li.prev == li.next && li.next == &li);

    cf_list_deinit(&li);
}


int main(int argc, char* argv[]) {
    test_list();
    return 0;
}
