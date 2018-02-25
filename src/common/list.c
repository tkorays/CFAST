#include <cf/types.h>
#include <cf/list.h>
#include <cf/memory.h>
#include <cf/err.h>


typedef struct _cf_list_node {
    cf_void_t*  data;
    struct _cf_list_node *prev, *next;
} cf_list_node_t;

struct cf_list {
    cf_size_t           number;     /*< number of list items */
    cf_void_t*          head;       /*< head of list */
    cf_void_t*          tail;       /*< tail of list */
    fn_cf_list_free     fn_free;    /*< free function pointer */
};


cf_list_t*  cf_list_create(fn_cf_list_free func) {
    struct cf_list* li = (struct cf_list*)cf_malloc(sizeof(struct cf_list));
    if(CF_NULL_PTR == li) {
        return CF_NULL_PTR;
    }

    li->number = 0;
    li->head = CF_NULL_PTR;
    li->tail = CF_NULL_PTR;
    li->fn_free = func;
    
    return li;
}

cf_errno_t cf_list_insert(struct cf_list* li, cf_void_t* data, cf_int32_t pos) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_list_node_t* tmp = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;
    if(!li || !data) return CF_EPARAM;

    node = (cf_list_node_t*)cf_malloc(sizeof(cf_list_node_t));
    node->data = data;

    abs_pos = (pos >= 0 ? pos : -pos);
    if((pos > 0 && abs_pos > li->number) && (pos < 0 && (abs_pos + 1) > li->number)) {
        cf_free(node);
        return CF_NOK;
    }

    if(pos < 0) {
        abs_pos = li->number + 1 - abs_pos;
    }

    /* 往前插入 */
    for(index = 0, tmp = li->head; index < abs_pos && tmp; index++, tmp = tmp->next) ;
    if(CF_NULL_PTR == li->head) {
        /* empty */
        node->prev = CF_NULL_PTR;
        node->next = CF_NULL_PTR;
        li->head = node;
        li->tail = node;
    }else if(index == 0) {
        /* head */
        node->prev = CF_NULL_PTR;
        node->next = tmp;
        tmp->prev = node;
        li->head = node;
    } else if(index >= li->number) {
        /* tail */
        node->prev = li->tail;
        node->next = CF_NULL_PTR;
        ((cf_list_node_t*)li->tail)->next = node;
        li->tail = node;
    } else {
        node->prev = tmp;
        node->next = tmp->next;
        tmp->next->prev = node;
        tmp->next = node;
    }
    li->number++;

    return CF_OK;
}

cf_errno_t cf_list_remove(struct cf_list* li, cf_void_t** data, cf_int32_t pos) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_list_node_t* tmp = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;
    if(!li || !data) return CF_EPARAM;

    abs_pos = (pos >= 0 ? pos : -pos);
    if((pos > 0 && abs_pos > li->number) && (pos < 0 && (abs_pos + 1) > li->number)) {
        return CF_NOK;
    }

    if(pos < 0) {
        abs_pos = li->number + 1 - abs_pos;
    }

    for(index = 0, node = li->head; index < abs_pos && node; index++, node = node->next) ;
    if(CF_NULL_PTR == li->head) {
        data = CF_NULL_PTR;
        return CF_NOK;
    } else if(index == 0) {
        /* head */
        li->head = node->next;
        if(node->next) {
            node->next->prev = CF_NULL_PTR;
        } else {
            li->tail = CF_NULL_PTR;
        }
    } else if(index >= li->number) {
        /* tail */
        node = li->tail;
        ((cf_list_node_t*)li->tail)->next = CF_NULL_PTR;
        li->tail = node->prev;
        if(node->prev) {
            node->prev->next = CF_NULL_PTR;
        } else {
            li->head = CF_NULL_PTR;
        }
    } else {
        tmp = node->prev;
        tmp->next = node->next;
        node->next->prev = tmp;
    }
    li->number--;
    *data = node->data;
    cf_free(node);

    return CF_OK;
}

cf_errno_t cf_list_delete(struct cf_list* li, cf_int32_t pos, cf_bool_t free_data) {
    cf_errno_t ret = CF_OK;
    cf_void_t* data = CF_NULL_PTR;
    if(!li) return CF_EPARAM;
    if(!li->fn_free && free_data) return CF_EPARAM;

    ret = cf_list_remove(li, &data, pos);
    if(ret != CF_OK) {
        return CF_NOK;
    }
    if(free_data) {
        li->fn_free(data);
    }

    return CF_OK;
}

cf_errno_t cf_list_destroy(struct cf_list* li, cf_bool_t free_data) {
    cf_list_iter_t it = CF_NULL_PTR;
    cf_list_node_t* node = CF_NULL_PTR;
    if(!li) return CF_EPARAM;
    cf_list_iter_init(li, &it);
    while(it) {
        node = it;
        it = cf_list_iter_next(it);

        if(free_data && li->fn_free) {
            li->fn_free(node->data);
        }
        cf_free(node);
    }
    cf_free(li);

    return CF_OK;
}

cf_size_t cf_list_size(struct cf_list* li) {
    if(!li) return 0;
    return li->number;
}

cf_errno_t cf_list_iter_init(struct cf_list* li, cf_list_iter_t* it) {
    if(!li || !it) return CF_EPARAM;
    *it = (cf_list_iter_t)li->head;
    return CF_OK;
}

cf_list_iter_t cf_list_iter_next(cf_list_iter_t it) {
    if(!it) return CF_NULL_PTR;
    return ((cf_list_node_t*)it)->next;
}

cf_errno_t cf_list_iter_data(cf_list_iter_t it, cf_void_t** data) {
    if(!it || !data) return CF_EPARAM;
    *data = ((cf_list_node_t*)it)->data;
    return CF_OK;
}
