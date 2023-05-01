
#include "cf/hashtbl.h"
#include "cf/assert.h"
#include <stdio.h>

cf_void_t cb(cf_void_t* value) {
    printf("%p\n", value);
}

int main(int argc, char *argv[]) {
    cf_hashtbl_iter_t it;
    cf_hashtbl_t* tbl = cf_hashtbl_new(15, cb);
    cf_void_t* p1 = CF_TYPE_CAST(cf_void_t*, 1);
    cf_void_t* p2 = CF_TYPE_CAST(cf_void_t*, 2);
    cf_hashtbl_set_by_hash(tbl, 5, p1);
    cf_assert(p1 == cf_hashtbl_get_by_hash(tbl, 5));
    cf_hashtbl_set_by_hash(tbl, 9, p2);
    cf_assert(p2 == cf_hashtbl_get_by_hash(tbl, 9));
    cf_hashtbl_set_by_hash(tbl, 5, p1);
    cf_assert(p1 == cf_hashtbl_get_by_hash(tbl, 5));

    cf_hashtbl_set(tbl, "abc", CF_HASH_STRING_KEY_LEN_AUTO, p1);
    cf_assert(p1 == cf_hashtbl_get(tbl, "abc", CF_HASH_STRING_KEY_LEN_AUTO));

    /** don't mix set and set_by_hash */
    cf_hashtbl_set_by_hash(tbl, 6, p2);
    cf_assert(p2 == cf_hashtbl_get_by_hash(tbl, 6));

    it = cf_hashtbl_iter_init(tbl);

    while (!cf_hashtbl_iter_end(tbl, it)) {
        if (cf_hashtbl_iter_key(it)) {
            printf("%s  \n", (const char*)cf_hashtbl_iter_key(it));
        }
        printf("%d:%p\n", cf_hashtbl_iter_hash(it), cf_hashtbl_iter_value(it));
        it = cf_hashtbl_iter_next(tbl, it);
    }

    cf_hashtbl_delete(tbl);
    return 0;
}

