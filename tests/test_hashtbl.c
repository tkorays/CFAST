
#include "cf/hashtbl.h"
#include "cf/assert.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    cf_hashtbl_t* tbl = cf_hashtbl_new(15);
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
    return 0;
}

