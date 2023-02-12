#include "cfx/fec.h"
#include "cf/memory.h"
#include "cf/list.h"


cf_bool_t cfx_xor_fec_encode(const cf_list_t* input,
                             const cfx_fec_param_t* param,
                             cf_uint8_t* mask,
                             cf_list_t* output);
cf_bool_t cfx_rs_fec_encode(cf_bool_t use_cauchy_matrix,
                            const cf_list_t* input,
                            const cfx_fec_param_t* param,
                            cf_uint8_t* mask,
                            cf_list_t* output);

struct cfx_fec {
    CFX_FEC_CREATE_TYPE instance_type;
    CFX_FEC_TYPE        fec_type;
    cfx_fec_param_t     param;
    cf_list_t           input;
    cf_list_t           output;
};

cfx_fec_t* cfx_fec_new(CFX_FEC_CREATE_TYPE create, CFX_FEC_TYPE type) {
    cfx_fec_t* fec = CF_NULL_PTR;

    if (type < CFX_FEC_TYPE_DEFAULT || type >= CFX_FEC_TYPE_NUM) {
        return CF_NULL_PTR;
    }
    if (create != CFX_FEC_DECODER || create != CFX_FEC_ENCODER) {
        return CF_NULL_PTR;
    }

    fec = cf_malloc_z(sizeof(cfx_fec_t));
    if (!fec) {
        return CF_NULL_PTR;
    }

    cf_list_init(&fec->input);
    cf_list_init(&fec->output);

    fec->instance_type = create;
    fec->fec_type = type;
    fec->param.num_of_data_blocks = 0;
    fec->param.num_of_fec_blocks = 0;

    return fec;
}

void cfx_fec_delete(cfx_fec_t* self) {
    if (self) {
        cf_free(self);
    }
}

cf_bool_t cfx_fec_is_encoder(cfx_fec_t* self) {
    return self->instance_type == CFX_FEC_ENCODER;
}

cf_bool_t cfx_fec_set_param(cfx_fec_t* self, const cfx_fec_param_t* param) {
    self->param = *param;
    return CF_TRUE;
}

cf_bool_t cfx_fec_encode(cfx_fec_t* self, const void* data, cf_size_t size) {
    if (self->param.num_of_fec_blocks == 0 ||
        self->param.num_of_data_blocks == 0) {
        return CF_TRUE;
    }

    if (!data) return CF_FALSE;

    // fix lint, we will not modify data
    cf_list_insert(&self->input, CF_LIST_POS_TAIL, (void*)data);

    if (cf_list_size(&self->input) >= self->param.num_of_data_blocks) {
        if (self->fec_type == CFX_FEC_TYPE_XOR) {
            cfx_xor_fec_encode(&self->input,
                               &self->param,
                               CF_NULL_PTR,
                               &self->output);
        } else if (self->fec_type == CFX_FEC_TYPE_RS_V || self->fec_type == CFX_FEC_TYPE_RS_C) {

        }
    }

    return CF_TRUE;
}

cf_bool_t cfx_fec_encode_output(cfx_fec_t* self, void* data, cf_size_t size) {
    if (!data) {
        // invalid buffer, we will remove one fec anyway.
        cf_list_erase(&self->output, CF_LIST_POS_HEAD);
        return CF_FALSE;
    }

    if (cf_list_size(&self->output) > 0) {
        /** cf_memcpy_s(data, size, cf_list_head(&self->output), 0); */
        cf_list_erase(&self->output, CF_LIST_POS_HEAD);
        return CF_TRUE;
    }
    return CF_FALSE;
}

cf_bool_t cfx_fec_decode(cfx_fec_t* self, const void* data, cf_size_t size) {

}

cf_bool_t cfx_fec_decode_output(cfx_fec_t* self, void* data, cf_size_t size) {
    if (!data) {
        // invalid buffer, we will remove one fec anyway.
        cf_list_erase(&self->output, CF_LIST_POS_HEAD);
        return CF_FALSE;
    }

    if (cf_list_size(&self->output) > 0) {
        /** cf_memcpy_s(data, size, cf_list_head(&self->output), 0); */
        cf_list_erase(&self->output, CF_LIST_POS_HEAD);
        return CF_TRUE;
    }
    return CF_FALSE;
}

