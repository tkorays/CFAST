#ifndef __CFAST_LIST_H__
#define __CFAST_LIST_H__

#include <stdlib.h>

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

#endif /* __CFAST_LIST_H__ */
