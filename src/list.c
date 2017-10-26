#include "cfast/list.h"
#include <stdlib.h>

cf_list_t* cf_list_new() {
    cf_list_t* li = (cf_list_t*)malloc(sizeof(cf_list_t));
    if(!li) return CF_NULL;
    li->list_size = 0;
    li->first = CF_NULL;
    li->current = CF_NULL;
    return li;
}

void cf_list_free(cf_list_t** li) {
    cf_node_t *p, *t;
    if(!li || !*li) return ;
    p = (*li)->first;
    while(p) {
        t = p->next;
        cf_node_free(&p);
        p = t;
    }
    free(*li);
    *li = CF_NULL;
}

cf_node_t* cf_list_new_node(cf_list_t* li, cf_size_t size) {
    cf_ret_t ret = cf_ok;
    cf_node_t* n = cf_node_new(size);
    if(!n) return CF_NULL;
    if(cf_ok != cf_list_add(li, n)) {
        cf_node_free(&n);
        return CF_NULL;
    }
    return n;
}

cf_ret_t cf_list_add(cf_list_t* li, cf_node_t* n) {
    if(!li || !n) return cf_err;
    if(li->first && !li->current) return cf_err; /* 链表错误 */
    n->next = CF_NULL;
    if(!li->first)  li->first = n; 
    else li->current->next = n;
    li->current = n;
    n->next = CF_NULL;
    li->list_size++;
    return cf_ok;
}

cf_node_t* cf_list_foreach(cf_list_t*li, cf_bool_t(*pfn)(cf_node_t* n, void* d), void*d) {
    cf_node_t *p, *t;
    if(!li || !pfn || !li->first || (li->first && !li->current)) return CF_NULL;
    p = li->first;
    while(p) {
        t = p->next;
        /* 如果函数返回true，表示foreach需要break，返回下一个节点 */
        if(cf_true == pfn(p, d)) return t;
        p = t;
    }
    return CF_NULL;
}

void cf_list_rebase(cf_list_t*li, cf_node_t*n) {
    cf_node_t *p, *t;
    if(!li) return ;
    li->list_size = 0;
    li->first = n;
    p = n;
    while(p) {
        li->list_size++;
        t = p->next;
        if(!t) break;
        p = t;
    }
    li->current = p;
}

cf_node_t* cf_node_new(cf_size_t size) {
    cf_node_t* n = (cf_node_t*)malloc(sizeof(cf_node_t));
    if(!n) return CF_NULL;
    n->data = (char*)malloc(size);
    if(!n->data) {
        free(n);
        return CF_NULL;
    }
    n->size = size;
    n->next = CF_NULL;
    return n;
}

void cf_node_free(cf_node_t** n) {
    if(!n || !*n) return;
    if((*n)->data) free((*n)->data);
    free(*n);
    *n = CF_NULL;
}
