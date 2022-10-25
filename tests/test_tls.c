#include "cf/tls.h"
#include "cf/assert.h"


int main(int argc, char* argv[]) {
    int a = 1234;
    cf_tls_t tls;
    cf_tls_init(&tls, CF_NULL_PTR);
    cf_tls_set(&tls, &a);
    int* p = (int*)cf_tls_get(&tls);
    cf_assert(a ==*p);
    cf_tls_deinit(&tls);
    return 0;
}