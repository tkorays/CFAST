#include <cfast/cf_que_if.h>
#include <cfast/cf_err_if.h>
#include <stdio.h>

int main() {
    cf_uint32_t arr[] = {1,2,3,4,5,6,7};
    cf_errno_t err;
    cf_char_t errs[64] = {0};
    cf_que_t* que = cf_que_create(5);
    cf_uint32_t *out, size;
    err = cf_que_enqueue(que, arr+0, sizeof(cf_uint32_t));
    
    err = cf_que_enqueue(que, arr+1, sizeof(cf_uint32_t));
    
    cf_que_enqueue(que, arr+2, sizeof(cf_uint32_t));
    cf_que_enqueue(que, arr+3, sizeof(cf_uint32_t));
    err = cf_que_enqueue(que, arr+4, sizeof(cf_uint32_t));
    err = cf_que_enqueue(que, arr+5, sizeof(cf_uint32_t));
    cf_err_get_info(err, errs, sizeof(errs));
    printf("err:%s\n", errs);
    
    cf_que_dequeue(que, (cf_void_t**)&out, &size);printf("out: %u, size: %u\n", *out, size);
    cf_que_dequeue(que, (cf_void_t**)&out, &size);printf("out: %u, size: %u\n", *out, size);
    cf_que_dequeue(que, (cf_void_t**)&out, &size);printf("out: %u, size: %u\n", *out, size);
    cf_que_dequeue(que, (cf_void_t**)&out, &size);printf("out: %u, size: %u\n", *out, size);
    cf_que_dequeue(que, (cf_void_t**)&out, &size);printf("out: %u, size: %u\n", *out, size);
    err = cf_que_dequeue(que, (cf_void_t**)&out, &size);
    cf_err_get_info(err, errs, sizeof(errs));
    printf("err:%s\n", errs);
    return 0;
}