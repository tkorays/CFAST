#ifndef __CFX_FEC_H__
#define __CFX_FEC_H__

#include "cf/types.h"

/**
 * @addgroup CFX_FEC fec
 * FEC 
 *
 * @{
 */

CF_DECLS_BEGIN

/**
 * FEC type
 */
typedef enum {
    CFX_FEC_TYPE_DEFAULT = 0,   /// default, RS FEC
    CFX_FEC_TYPE_RS_V,          /// Reed-Solomon FEC with Van der Monde matrix
    CFX_FEC_TYPE_RS_C,          /// Reed-Solomon FEC with Cauchy matrix
    CFX_FEC_TYPE_XOR,           /// XOR FEC, FlexFEC see RFC 5109
    CFX_FEC_TYPE_RAPTOR,        /// Raptor FEC, not implemented
    CFX_FEC_TYPE_NUM,           /// number of fec types
} CFX_FEC_TYPE;

/**
 * to simply the api, encoder and decoder share the same interface
 * we use the create type to distinguish them
 */
typedef enum {
    CFX_FEC_ENCODER,
    CFX_FEC_DECODER,
} CFX_FEC_CREATE_TYPE;

/**
 * FEC encoder and decoder instance
 */
typedef struct cfx_fec cfx_fec_t;

/**
 * FEC parameters
 */
typedef struct {
    cf_size_t num_of_data_blocks;
    cf_size_t num_of_fec_blocks;
} cfx_fec_param_t;

/**
 * create a new FEC instance.
 *
 * @param   type    select which FEC codec
 * @param   create  create a decoder or a encoder
 * @return new instance
 */
cfx_fec_t* cfx_fec_new(CFX_FEC_CREATE_TYPE create, CFX_FEC_TYPE type);
void cfx_fec_delete(cfx_fec_t* self);
cf_bool_t cfx_fec_is_encoder(cfx_fec_t* self);
cf_bool_t cfx_fec_set_param(cfx_fec_t* self, const cfx_fec_param_t* param);
cf_bool_t cfx_fec_encode(cfx_fec_t* self, const void* data, cf_size_t size);
cf_bool_t cfx_fec_encode_output(cfx_fec_t* self, void* data, cf_size_t size);
cf_bool_t cfx_fec_decode(cfx_fec_t* self, const void* data, cf_size_t size);
cf_bool_t cfx_fec_decode_output(cfx_fec_t* self, void* data, cf_size_t size);


CF_DECLS_END

/** @} */

#endif /* __CFX_FEC_H__ */

