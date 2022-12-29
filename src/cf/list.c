#include <cf/types.h>
#include <cf/list.h>
#include <cf/memory.h>
#include <cf/err.h>
#include <cf/assert.h>


typedef struct _cf_list_node {
    cf_void_t*  data;
    struct _cf_list_node *prev, *next;
} cf_list_node_t;


cf_bool_t cf_list_init(cf_list_t* self) {
    self->count = 0;
    self->head = CF_NULL_PTR;
    self->tail = CF_NULL_PTR;
    return CF_TRUE;
}

cf_void_t cf_list_deinit(cf_list_t* self) {
    cf_list_iter_t it = CF_NULL_PTR;
    cf_list_node_t* node = CF_NULL_PTR;
    it = cf_list_iter_init(self);
    while(it) {
        node = it;
        it = cf_list_iter_next(it);

        cf_free(node);
    }

    self->head = CF_NULL_PTR;
    self->tail = CF_NULL_PTR;
    self->count = 0;
}


cf_bool_t cf_list_insert(cf_list_t* li, cf_int32_t pos, cf_void_t* data) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_list_node_t* tmp = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;
    if(!data) return CF_FALSE;

    /* check insert position */
    abs_pos = (pos >= 0 ? pos : li->count + 1 + pos);
    if (abs_pos < 0 || abs_pos > li->count) {
        return CF_FALSE;
    }

    node = (cf_list_node_t*)cf_malloc(sizeof(cf_list_node_t));
    node->data = data;

    /* insert front */
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
    } else if(index >= li->count - 1) {
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
    li->count++;

    return CF_TRUE;
}

cf_void_t* cf_list_erase(struct cf_list* li, cf_int32_t pos) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_list_node_t* tmp = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;
    cf_void_t* data = CF_NULL_PTR;

    /* check insert position */
    abs_pos = (pos >= 0 ? pos : li->count + 1 + pos);
    if (abs_pos < 0 || abs_pos > li->count) {
        return CF_NULL_PTR;
    }

    for(index = 0, node = li->head; index < abs_pos && node; index++, node = node->next) ;
    if(CF_NULL_PTR == li->head) {
        return CF_NULL_PTR;
    } else if(index == 0) {
        /* head */
        li->head = node->next;
        if(node->next) {
            node->next->prev = CF_NULL_PTR;
        } else {
            li->tail = CF_NULL_PTR;
        }
    } else if(index >= li->count) {
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
    li->count--;
    data = node->data;
    cf_free(node);

    return data;
}

cf_void_t* cf_list_get(cf_list_t* li, cf_int32_t pos) {
    cf_list_node_t* node = CF_NULL_PTR;
    cf_uint32_t abs_pos = 0;
    cf_uint32_t index;

    /* check insert position */
    abs_pos = (pos >= 0 ? pos : li->count + pos);
    if (abs_pos < 0 || abs_pos > li->count - 1) {
        return CF_NULL_PTR;
    }

    for(index = 0, node = li->head; index < abs_pos && node; index++, node = node->next) ; 
    if(CF_NULL_PTR == node) {
        return CF_NULL_PTR;
    } else {
        return node->data;
    }
}

cf_void_t* cf_list_head(cf_list_t* li) {
    if (li->head) {
        return CF_TYPE_CAST(cf_list_node_t*, li->head)->data;
    }
    return CF_NULL_PTR;
}

cf_void_t* cf_list_tail(cf_list_t* li) {
    if (li->tail) {
        return CF_TYPE_CAST(cf_list_node_t*, li->tail)->data;
    }
    return CF_NULL_PTR;
}

cf_list_iter_t cf_list_iter_init(struct cf_list* li) {
    return (cf_list_iter_t)li->head;
}

cf_list_iter_t cf_list_iter_next(cf_list_iter_t it) {
    if(!it) return CF_NULL_PTR;
    return ((cf_list_node_t*)it)->next;
}

cf_void_t* cf_list_iter_data(cf_list_iter_t it) {
    return ((cf_list_node_t*)it)->data;
}
