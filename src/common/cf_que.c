#include "cfast/cf_que_if.h"

cf_que_t* cf_que_create(cf_size_t max_size);
cf_errno_t  cf_que_destroy(cf_que_t* que);
cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size);
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size);