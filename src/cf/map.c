#include "cf/map.h"
#include "cf/memory.h"
#include "cf/rbtree.h"
#include "cf/types.h"
#include <string.h>

typedef struct {
    struct rb_node node;
    void*       key;
    cf_size_t   key_len;
    void*       value;
} map_item;

struct cf_map {
    struct rb_root* root;
};
cf_map_t* cf_map_new() {
    cf_map_t* m = cf_malloc_z(sizeof(cf_map_t));
    return m;
}
void cf_map_delete(cf_map_t* self) {
    if (self) {
        cf_free(self);
    }
}

cf_size_t cf_map_size(cf_map_t* self) {
    return 0;
}

void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len) {
    struct rb_node *node = self->root->rb_node; 
    while (node) {
        map_item *data = CF_TYPE_CAST(map_item*, node);

        //compare between the key with the keys in map
        int cmp = memcmp(key, data->key, len);
        if (cmp < 0) {
            node = node->rb_left;
        }else if (cmp > 0) {
            node = node->rb_right;
        }else {
            return data;
        }
   }
   return NULL;
}

void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value) {
    map_item *data = (map_item*)cf_malloc_z(sizeof(map_item));
    data->key = cf_malloc_z(len);
    cf_memcpy_s(data->key, len, key, len);
    data->value = value;
    
    struct rb_node **new_node = &(self->root->rb_node), *parent = NULL;
    while (*new_node) {
        map_item* this_node = CF_TYPE_CAST(map_item*, *new_node);
        int result = strcmp(key, this_node->key);
        parent = *new_node;

        if (result < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (result > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            this_node->value = value;
            cf_free(data->key);
            cf_free(data);
            return;
        }
    }

    rb_link_node(&data->node, parent, new_node);
    rb_insert_color(&data->node, self->root);
}

cf_map_iter_t cf_map_iter_init(cf_map_t* self);
cf_map_iter_t cf_map_iter_next(cf_map_iter_t it);
cf_bool_t cf_map_iter_end(cf_map_iter_t it);
void* cf_map_iter_value(cf_map_iter_t it);

