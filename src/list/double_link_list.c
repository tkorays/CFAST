#include "cfast/cf_def.h"
#include "cfast/cf_list_if.h"


typedef struct _cf_list_node {
    cf_void_t*  data;
    struct _cf_list_node *prev, *next;
} cf_list_node_t;


cf_list_t*  cf_list_new(fn_cf_list_free func) {
    cf_list_t* li = (cf_list_t*)malloc(sizeof(cf_list_t));
    if(CF_NULL_PTR == li) {
        return CF_NULL_PTR;
    }

    li->number = 0;
    li->head = CF_NULL_PTR;
    li->tail = CF_NULL_PTR;
    li->fn_free = func;
    
    return li;
}

cf_ret_t cf_list_insert(cf_list_t* li, cf_void_t* data, cf_int32_t pos) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_list_node_t* tmp = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;
    if(!li || !data) return CF_RET_NULL_PTR;

    node = (cf_list_node_t*)malloc(sizeof(cf_list_node_t));
    node->data = data;

    if(CF_NULL_PTR == li->head) {
        node->prev = CF_NULL_PTR;
        node->prev = CF_NULL_PTR;
        li->head = node;
        li->tail = node;
        li->number++;
        return CF_RET_SUCCESS;
    }

    if(CF_LIST_POS_HEAD == pos) {
        tmp = li->head;
        node->prev = CF_NULL_PTR;
        node->next = tmp;
        tmp->prev = node;
        li->head = node;
        li->number++;
        return CF_RET_SUCCESS;
    } 

    if(CF_LIST_POS_TAIL == pos) {
        tmp = li->tail;
        node->prev = tmp;
        node->next = CF_NULL_PTR;
        tmp->next = node;
        li->tail = node;
        li->number++;
        return CF_RET_SUCCESS;
    }

    abs_pos = (pos >= 0 ? pos : -pos);
    if(abs_pos > li->number) {
        free(node);
        return CF_RET_FAIL;
    }

    if(pos < 0) {
        abs_pos = li->number - abs_pos + 1;
    }

    for(index = 0, tmp = li->head; index < abs_pos && tmp; index++, tmp = tmp->next) ;
    if(!tmp) {
        free(node);
        return CF_RET_FAIL;
    }

    node->prev = tmp;
    node->next = tmp->next;
    if(tmp->next) {
        tmp->next->prev = node;
    }
    tmp->next = node;
    li->number++;

    return CF_RET_SUCCESS;
}

cf_ret_t    cf_list_remove(cf_list_t* li, cf_void_t* data, cf_int32_t pos) {

}
cf_ret_t cf_list_free(cf_list_t* li, cf_bool_t free_node) {
    cf_list_iter_t it = CF_NULL_PTR;
    cf_list_node_t* node = CF_NULL_PTR;
    if(!li) return CF_RET_FAIL;
    cf_list_iter_init(li, &it);
    while(it) {
        node = it;
        it = cf_list_iter_next(it);

        if(free_node && li->fn_free) {
            li->fn_free(node->data);
        }
        free(node);
    }
    free(li);

    return CF_RET_SUCCESS;
}

cf_ret_t    cf_list_iter_init(cf_list_t* li, cf_list_iter_t* it) {
    if(!li || !it) return CF_RET_NULL_PTR;
    *it = (cf_list_iter_t)li->head;
    return CF_RET_SUCCESS;
}

cf_list_iter_t  cf_list_iter_next(cf_list_iter_t it) {
    if(!it) return CF_NULL_PTR;
    return ((cf_list_node_t*)it)->next;
}

cf_ret_t    cf_list_iter_data(cf_list_iter_t it, cf_void_t* data) {
    if(!it || !data) return CF_RET_NULL_PTR;
    data = ((cf_list_node_t*)it)->data;
    return CF_RET_SUCCESS;
}
