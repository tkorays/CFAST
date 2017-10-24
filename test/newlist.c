
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/cftype.h"

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

cf_bool_t pa(cf_node_t* n, void* dt) {
    printf("size: %u\n", n->size);
    return cf_false;
}
cf_bool_t mv(cf_node_t* n, void* dt) {
    cf_list_t** ll = (cf_list_t**)dt;
    cf_list_rebase(ll[0], n->next);
    cf_list_add(ll[1], n);
    return cf_false;
}

cf_bool_t zip(cf_node_t*n, void* dt) {
    return cf_true;
}

cf_bool_t keep_list_size() {
    return cf_true;
}

/**
 * 一下实现一个路径管理 
 */
void test() {
    cf_list_t* fli = cf_list_new();
    cf_list_t* his = cf_list_new();
    cf_node_t* n1 = cf_list_new_node(fli, 256);
    cf_node_t* n2 = cf_list_new_node(fli, 256);

    strcpy(n1->data, "abc.txt");
    strcpy(n1->data, "abc1.txt");

    cf_list_t* ll[] = {fli, his};
    /* 执行压缩，然后把文件加入历史记录 */
    cf_list_foreach(fli, zip, ll);

}

int main() {
    cf_list_t* li = cf_list_new();
    printf("new list, size: %u\n", li->list_size);

    cf_node_t* n1 = cf_list_new_node(li, 2);
    cf_node_t* n2 = cf_list_new_node(li, 3);
    printf("new list, size: %u\n", li->list_size);

    cf_list_foreach(li, pa, CF_NULL);

    cf_list_t* l2 = cf_list_new();

    cf_list_t* ll[] = {li, l2};
    cf_list_foreach(li, mv, ll);

    printf("new list, size: %u, %u\n", li->list_size, l2->list_size);

    cf_list_free(&li);
    cf_list_free(&l2);
    return 0;
}