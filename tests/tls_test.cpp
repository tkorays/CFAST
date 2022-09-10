#include <gtest/gtest.h>
#include <cf/tls.h>

int a = 1234;

TEST(cf_tls_test, test) {
    cf_tls_t tls;
    cf_tls_init(&tls, CF_NULL_PTR);
    cf_tls_set(&tls, &a);
    int* p = (int*)cf_tls_get(&tls);
    EXPECT_EQ(a,*p);
    cf_tls_deinit(&tls);
}