#include <cfast/cf_mpool_if.h>
#include <stdio.h>

int main() {
    cf_mpool_t* pool = cf_mpool_create(16);
    printf("pool: %p\n", pool);
    cf_char_t* s1 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s1);
    cf_char_t* s2 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s2);
    cf_char_t* s3 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s3);
    cf_char_t* s4 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s4);
    cf_char_t* s5 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s5);
    cf_char_t* s6 = cf_mpool_alloc(pool, 3);printf("addr: %p\n", s6);
    cf_mpool_destroy(pool);
    return 0;
    return 0;
}