#ifndef __CFAST_LIST_H__
#define __CFAST_LIST_H__

#include "cftype.h"

typedef struct cf_node {
    char* data;
    cf_size_t size;
    struct cf_node* next;
} cf_node_t;

typedef struct {
    cf_size_t list_size;
    cf_node_t* first;
    cf_node_t* current;
} cf_list_t;

cf_list_t* cf_list_new();
void cf_list_free(cf_list_t** li);
cf_node_t* cf_list_new_node(cf_list_t* li, cf_size_t size);
cf_ret_t cf_list_add(cf_list_t* li, cf_node_t* n);
cf_node_t* cf_list_foreach(cf_list_t*li, cf_bool_t(*pfn)(cf_node_t* n, void* d), void*);
void cf_list_rebase(cf_list_t*li, cf_node_t*n);

cf_node_t* cf_node_new(cf_size_t size);
void cf_node_free(cf_node_t** n);

#endif /* __CFAST_LIST_H__ */
