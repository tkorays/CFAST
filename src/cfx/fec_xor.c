#include "fec_xor.h"
#include "fec_xor_tbl.h"
#include "cf/assert.h"
#include "cf/memory.h"
#include <stdio.h>


cf_size_t get_mask_index(cf_size_t n, cf_size_t k) {
    cf_size_t index = 1;
    cf_size_t i = 1;
    while (i < n) {
        index += 1;
        index += i * (i + 1);
        i++;
    }
    printf("n: %lld, k: %lld, index:%lld, exp: %d\n", n, k, index, FEC_MASK_TBL_RANDOM[index]);
    cf_assert(n == FEC_MASK_TBL_RANDOM[index]);
    index += 1;
    index += k * (k - 1);
    printf("->%lld\n", index);
    return index;
}

int cfx_fec_xor_encode(void* self,
                       const cf_iovec_t* input,
                       cf_size_t n,
                       cf_iovec_t* output,
                       cf_size_t k) {
    cf_size_t index = 0;

    if (input == CF_NULL_PTR || output == CF_NULL_PTR) {
        return -1;
    }
    if (n == 0 || n > CFX_FEC_XOR_MAX_N) {
        return -1;
    }
    if (k == 0 || k > n) {
        return -1;
    }

    index = get_mask_index(n, k);

    cf_uint16_t mask = 0;
    cf_size_t i = 0;
    for (i = 0; i < k; i++) {
        cf_membzero(output[i].iov_base, output[i].iov_len);

        mask = (FEC_MASK_TBL_RANDOM[index] << 8 | FEC_MASK_TBL_RANDOM[index + 1]);
        printf("mask:%x\n", mask);
        cf_size_t j = 0;
        while (j < n) {
            if ((mask << j) & 0x8000) {
                printf(">> j: %lld i: %lld\n", j, i);
                cf_size_t a = 0;
                while (a < input[j].iov_len) {
                    *((cf_uint8_t*)output[i].iov_base + a) ^= *((cf_uint8_t*)input[j].iov_base + a);
                    a++;
                }
            }
            j++;
        }
        index += 2;
    }

    return 0;
}

cf_bool_t cfx_fec_xor_decodable(void* self, const cf_iovec_t* input, cf_size_t n_plus_k) {
    return CF_TRUE;
}
        
int cfx_fec_xor_decode(void* self,
                       const cf_iovec_t* input,
                       cf_size_t n_plus_k,
                       cf_iovec_t* output,
                       cf_size_t n) {
    cf_size_t k = n_plus_k - n;
    cf_size_t index = 0;

    if (input == CF_NULL_PTR) {
        return -1;
    }
    if (n == 0 || n > CFX_FEC_XOR_MAX_N) {
        return -1;
    }
    if (k == 0 || k > n) {
        return -1;
    }
    
    // 先找到n k编码对应的mask index
    index = get_mask_index(n, k);

    cf_size_t i = 0;
    cf_uint16_t mask = 0;
    // 从k个FEC包，依次恢复
    for (i = 0; i < k; i++) {
        // not receive this protectted packet
        if (input[n + i].iov_base == CF_NULL_PTR) {
            continue;
        }

        // mask of the ith protectted packet
        mask = (FEC_MASK_TBL_RANDOM[index + 2 * i] << 8 | FEC_MASK_TBL_RANDOM[index + 2 * i + 1]);

        // 第一遍检索，确认是否只有一个包没有收到
        cf_size_t miss = 0;
        cf_size_t missing_index = 0;
        cf_size_t j = 0;
        while (j < n) {
            if ((mask << j) & 0x8000) {
                printf(">> j: %lld i: %lld\n", j, i);
                if (input[j].iov_base == CF_NULL_PTR) {
                    miss++;
                    missing_index = j;
                }
            }
            j++;
        }
        if (miss == 1) {
            printf("has one missing, index: %lld, try to decode, mask: %x\n", missing_index, mask);
            cf_memcpy_s(output[missing_index].iov_base, output[missing_index].iov_len,
                        input[n + i].iov_base, input[n + i].iov_len);
            j = 0;
            while (j < n) {
                if ((mask << j) & 0x8000 && input[j].iov_base != CF_NULL_PTR) {
                    printf(">> j: %lld i: %lld\n", j, i);
                    cf_size_t a = 0;
                    for (a = 0; a < input[j].iov_len; a++) {
                        *((cf_uint8_t*)output[missing_index].iov_base + a) ^= *((cf_uint8_t*)input[j].iov_base + a);
                    }
                }
                j++;
            }
        }
    }

    return 0;
}

