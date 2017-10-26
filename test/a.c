#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define CFLIST_STOP     1
#define CFLIST_CONTINUE 0

struct cflist;
struct cflist_node;

struct cflist* cflist_new();
void cflist_free(struct cflist**);
void cflist_add(struct cflist*, struct cflist_node*);
struct cflist_node* cflist_foreach(struct cflist*,int(*pfn)(struct cflist_node*, void*), void*);
void cflist_rebase_force(struct cflist*, struct cflist_node*);
size_t cflist_size(struct cflist*);

struct cflist_node* cflist_node_new(size_t);
void cflist_node_free(struct cflist_node**);
char* cflist_node_get(struct cflist_node*);
size_t cflist_node_size(struct cflist_node*);

////////////
struct cflist {
    size_t num;
    struct cflist_node* first;
    struct cflist_node* current;
};

struct cflist_node {
    size_t size;
    void* data;
    struct cflist_node* next;
};

struct cflist* cflist_new() {
    struct cflist* li = (struct cflist*)malloc(sizeof(struct cflist));
    if(!li) return 0;

    li->num = 0;
    li->first = 0;
    li->current = 0;
    return li;
}

void cflist_free(struct cflist** li) {
    struct cflist_node *n, *t;
    if(!li || !*li || !(*li)->first || !(*li)->current) return;
    n = (*li)->first;
    while(n) {
        t = n->next;
        cflist_node_free(&n);
        n = t;
    }
    free(*li);
    *li = 0;
}

void cflist_add(struct cflist* li , struct cflist_node* n) {
    if(!li || !n) return ;
    n->next = 0;
    if(!li->first) {
        li->first = n;
    } else {
        li->current->next = n;
    }
    li->current = n;
    li->num++;
}

struct cflist_node* cflist_foreach(struct cflist* li,
    int(*pfn)(struct cflist_node*, void*), 
    void* data) {
    struct cflist_node *n, *t;
    if(!li || !pfn || !li->first) return 0;
    n = li->first;
    while(n) {
        t = n->next;
        if(pfn(n, data) != 0) {
            return t;
        }
        n = t;
    }
    return 0;
}

void cflist_rebase_force(struct cflist* li, struct cflist_node* n) {
    size_t num = 0;
    if(!li) return ;
    if(!n) {
        li->num = 0;
        li->first = 0;
        li->current = 0;
        return ;
    }

    li->first = n;
    li->current = n;
    li->num = 1;
    while(li->current->next) {
        li->current = li->current->next;
        li->num++;
    }
}

size_t cflist_size(struct cflist* li)
{
    if(!li) return 0;
    return li->num;
}

struct cflist_node* cflist_node_new(size_t size) {
    struct cflist_node* n = (struct cflist_node*)malloc(sizeof(struct cflist_node));
    if(!n || !(n->data = malloc(size))) return 0;
    n->size = size;
    return n;
}

void cflist_node_free(struct cflist_node** n) {
    if(!n || !*n) return ;
    if((*n)->data) free((*n)->data);
    free(*n);
    *n = 0;
}

char* cflist_node_get(struct cflist_node* n) {
    if(!n) return 0;
    return (char*)n->data;
}
size_t cflist_node_size(struct cflist_node* n) {
    if(!n) return 0;
    return n->size;
}

int i = 0;
int fuck(struct cflist_node* n, void* dt)
{
    struct cflist** ll = (struct cflist**)dt;
    if(!n) return 0;
    i++;
    printf("reverse node\n");
    cflist_rebase_force(ll[0], n->next);
    cflist_add(ll[1], n);

    return CFLIST_CONTINUE;
}

/* 以下代码实现了将一个数组中的元素移动到另一个数组 */
int main() {
    struct cflist* l1 = cflist_new();
    struct cflist* l2 = cflist_new();
    struct cflist* ll[2] = {l1, l2};
    struct cflist_node* n = cflist_node_new(10);
    struct cflist_node* n2 = cflist_node_new(10);
    char* mem = cflist_node_get(n);
    size_t size = cflist_node_size(n);
    cflist_add(l1, n);
    cflist_add(l1, n2);
    printf("size of array : %zu, %zu\n", cflist_size(l1), cflist_size(l2));
    n = cflist_foreach(l1, fuck, ll);
    printf("%p\n", n);
    printf("size of array : %zu, %zu\n", cflist_size(l1), cflist_size(l2));
    cflist_free(&l1);
    cflist_free(&l2);
    return 0;
}
