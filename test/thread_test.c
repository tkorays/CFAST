#include <cf/thread.h>
#include <stdio.h>

CF_THREAD_DEF_PROC(abc, arg) {
    printf("thread enter!\n");
    cf_thread_exit(2345);
    return CF_NULL_PTR;
}

int main() {
    cf_thread_t t;
    cf_uint32_t retval;
    cf_thread_create(&t, CF_NULL_PTR, abc, CF_NULL_PTR);
    cf_thread_join(t, &retval);
    printf("thread ret: %u\n", retval);
    return 0;
}