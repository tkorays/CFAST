#include "cfx/fec.h"
#include "cf/assert.h"
#include <stdio.h>
#include "cf/types.h"
#include "../src/cfx/fec_xor.h"

int main(int argc, char *argv[])
{
    
    cfx_fec_xor_t fec;
    cf_iovec_t input[3] = {
        { "hello", 5},
        { "world", 5},
        { "whatthefuck", 11 }
    };

    cf_uint8_t o1[12];
    cf_uint8_t o2[12];
    cf_iovec_t output[2] = {
        {&o1[0], 12},
        {&o2[0], 12},
    };

    cfx_fec_xor_encode(&fec, &input, 3, &output, 2);
    cf_size_t i;
    for (i = 0; i < 12; i++) {
        printf("%d,", o1[i]);
    }
    printf("\n");
    for (i = 0; i < 12; i++) {
        printf("%d,", o2[i]);
    }
    printf("\n");


    cf_iovec_t recv[3+2] = {
        { "hello", 5 },
        { CF_NULL_PTR, 0 },
        { CF_NULL_PTR, 0 },
        { &o1[0], 12 },
        { &o2[0], 12 },
    };
    cf_uint8_t r1[20] = {0};
    cf_uint8_t r2[20] = {0};
    cf_iovec_t recover[3] = {
        {CF_NULL_PTR, 0},
        { &r1[0], 20},
        { &r2[0], 20},
    };

    cfx_fec_xor_decode(&fec, &recv, 5, &recover, 3);
    for (i = 0; i < 20; i++) {
        printf("%d,", r1[i]);
    }
    printf("\n");

    for (i = 0; i < 20; i++) {
        printf("%d,", r2[i]);
    }
    printf("\n");
    printf("%s, %s\n", (char*)r1, (char*)r2);
    return 0;
}
