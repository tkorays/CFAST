#ifndef __CFX_FEC_XOR_H__
#define __CFX_FEC_XOR_H__

#include "cf/types.h"


typedef struct {
    void * a;
} cfx_fec_xor_t;

int cfx_fec_xor_encode(void* self,
                       const cf_iovec_t* input,
                       cf_size_t n,
                       cf_iovec_t* output,
                       cf_size_t k);

cf_bool_t cfx_fec_xor_decodable(void* self, const cf_iovec_t* input, cf_size_t n_plus_k);
        
int cfx_fec_xor_decode(void* self,
                       const cf_iovec_t* input,
                       cf_size_t n_plus_k,
                       cf_iovec_t* output,
                       cf_size_t n);

#endif /* __CFX_FEC_XOR_H__ */

