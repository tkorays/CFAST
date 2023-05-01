
#include "cf/hashtbl.h"
#include "cf/assert.h"
#include <stdio.h>

cf_void_t cb(cf_void_t* value) {
    printf("%p\n", value);
}

void test_hashtbl_set_get() {
    int keys[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
    int values[] = {1, 2, 3, 4};
    cf_size_t i, cnt = 4;
    cf_hashtbl_t* tbl = cf_hashtbl_new(15, cb);
    for (i = 0; i < cnt; i++) {
        cf_hashtbl_set(tbl, &keys[i], sizeof(int), &values[i]);
    }

    cf_assert(cf_hashtbl_size(tbl) == cnt);

    for (i = 0; i < cnt; i++) {
        cf_assert(*(int*)cf_hashtbl_get(tbl, &keys[i], sizeof(int)) == values[i]);
    }
    cf_hashtbl_delete(tbl);
}

void test_hashtbl_iter() {
    int keys[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
    int values[] = {1, 2, 3, 4};
    cf_size_t i, cnt = 4;
    cf_hashtbl_iter_t it;
    cf_hashtbl_t* tbl = cf_hashtbl_new(15, cb);
    for (i = 0; i < cnt; i++) {
        cf_hashtbl_set(tbl, &keys[i], sizeof(int), &values[i]);
    }
    
    i = 0;
    for (it = cf_hashtbl_iter_init(tbl);
        !cf_hashtbl_iter_end(tbl, it);
        it = cf_hashtbl_iter_next(tbl, it)) {
        i++;
    }
    cf_assert(i == 4);

    cf_hashtbl_delete(tbl);
}

void test_hashtbl_remove() {
    int keys[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
    int values[] = {1, 2, 3, 4};
    cf_size_t i, cnt = 4;
    cf_hashtbl_t* tbl = cf_hashtbl_new(15, cb);
    for (i = 0; i < cnt; i++) {
        cf_hashtbl_set(tbl, &keys[i], sizeof(int), &values[i]);
    }
    
    cf_assert(cf_hashtbl_size(tbl) == cnt);
    cf_hashtbl_set(tbl, &keys[2], sizeof(int), CF_NULL_PTR);
    cf_assert(cf_hashtbl_size(tbl) == (cnt - 1));
    cf_assert(cf_hashtbl_get(tbl, &keys[2], sizeof(int)) == CF_NULL_PTR);

    cf_hashtbl_delete(tbl);
}

int main(int argc, char *argv[]) {
    test_hashtbl_set_get();
    test_hashtbl_iter();
    test_hashtbl_remove();
    return 0;
}

