#ifndef __CF_QUE_IF_H__
#define __CF_QUE_IF_H__

#include "cf_def.h"

typedef struct cf_que_s cf_que_t;

typedef struct {
    cf_size_t   size;
    cf_void_t*  data;
} cf_que_item_t;

cf_que_t* cf_que_create(cf_size_t max_size);
cf_errno_t  cf_que_destroy(cf_que_t* que);
cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size);
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size);

#endif /* __CF_QUE_IF_H__ */
