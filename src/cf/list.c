#include <cf/types.h>
#include <cf/list.h>
#include <cf/memory.h>
#include <cf/err.h>
#include <cf/assert.h>
#include <stdio.h>


cf_bool_t cf_list_init(cf_list_t* self) {
    self->data = 0;
    self->prev = self->next = self;
    return CF_TRUE;
}

cf_void_t cf_list_deinit(cf_list_t* self) {
    cf_list_iter_t it = CF_NULL_PTR;
    cf_list_node_t* node = CF_NULL_PTR;
    it = cf_list_iter_init(self);
    while(it != self) {
        node = it;
        it = it->next;

        cf_free(node);
    }

    self->prev = self->next = CF_NULL_PTR;
    self->data = 0;
}


cf_void_t cf_list_insert_before(cf_list_t* self, cf_list_iter_t iter, void* data) {
    cf_list_node_t* node = CF_NULL_PTR;

    /* prev[next] -> node[prev+next] -> iter[prev] */
    node = (cf_list_node_t*)cf_malloc_z(sizeof(cf_list_node_t));
    node->data = data;
    node->next = iter;
    node->prev = iter->prev;
    iter->prev->next = node;
    iter->prev = node;

    self->data = CF_TYPE_CAST(cf_void_t*, CF_TYPE_CAST(cf_uintptr_t, self->data) + 1);
}

cf_void_t cf_list_insert_after(cf_list_t* self, cf_list_iter_t iter, void* data) {
    cf_list_node_t* node = CF_NULL_PTR;

    /* iter[next] -> node[prev+next] -> next[prev] */
    node = (cf_list_node_t*)cf_malloc_z(sizeof(cf_list_node_t));
    node->data = data;
    node->prev = iter;
    node->next = iter->next;
    iter->next->prev = node;
    iter->next = node;

    self->data = CF_TYPE_CAST(cf_void_t*, CF_TYPE_CAST(cf_uintptr_t, self->data) + 1);
}

cf_void_t cf_list_erase(cf_list_t* self, cf_list_iter_t iter) {
    cf_list_node_t* node = iter;

    iter->prev->next =  iter->next;
    iter->next->prev = iter->prev;
    cf_free(node);

    self->data = CF_TYPE_CAST(cf_void_t*, CF_TYPE_CAST(cf_uintptr_t, self->data) - 1);
}

cf_list_iter_t cf_list_find(cf_list_t* self, void* data, cf_alg_equal_f eqf) {
    cf_list_iter_t it = cf_list_iter_init(self);

    while (!cf_list_iter_end(self, it)) {
        if (eqf(data, cf_list_iter_data(it))) {
            return it;
        }

        it = cf_list_iter_next(it);
    }
    return CF_NULL_PTR;
}
