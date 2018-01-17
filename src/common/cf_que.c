#include "cfast/cf_que_if.h"
#include "cfast/cf_err_if.h"
#include "cfast/cf_mem_if.h"

typedef struct {
    cf_size_t   size;
    cf_void_t*  data;
} cf_que_item_t;

typedef struct cf_que_s {
    cf_size_t       max_size;
    cf_que_item_t*  items;
    cf_uint_t       head;
    cf_uint_t       tail;
    cf_bool_t       empty;
} cf_que_t;


cf_que_t* cf_que_create(cf_size_t max_size) {
    cf_que_t* que = CF_NULL_PTR;
    if(max_size == 0) return CF_NULL_PTR;

    que = (cf_que_t*)cf_malloc(sizeof(cf_que_t));
    if(!que) return CF_NULL_PTR;

    que->items = (cf_que_item_t*)cf_malloc(sizeof(cf_que_item_t)*max_size);
    if(!que->items) {
        cf_free(que);
        return CF_NULL_PTR;
    }

    que->max_size = max_size;
    que->head = 0;
    que->tail = 0;
    que->empty = CF_TRUE;
    return que;
}

cf_errno_t  cf_que_destroy(cf_que_t* que) {
    if(!que) return CF_EPARAM;
    if(que->items) cf_free(que->items);
    cf_free(que);
    return CF_OK;
}

cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size) {
    if(!que || !data || size == 0) return CF_EPARAM;
    /* full */
    if(que->empty == CF_FALSE && que->tail == que->head) return CF_EFULL;

    que->items[que->tail].size = size;
    que->items[que->tail].data = data;

    que->tail = (que->tail + 1) % que->max_size;

    if(que->empty == CF_TRUE) que->empty = CF_FALSE;
    return CF_OK;
}
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size) {
    if(!que || !data || size == 0) return CF_EPARAM;
    if(que->empty == CF_TRUE) return CF_EEMPTY;

    *data = que->items[que->head].data;
    *size = que->items[que->head].size;
    que->head = (que->head + 1) % que->max_size;

    if(que->head == que->tail) que->empty = CF_TRUE;
    return CF_OK;
}
