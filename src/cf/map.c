#include "cf/map.h"
#include "cf/memory.h"
#include "cf/rbtree.h"
#include "cf/types.h"
#include "cf/value.h"
#include <string.h>

typedef struct {
    struct rb_node  node;
    cf_value_t      map_key;
    void*           value;
} map_item;

struct cf_map {
    struct rb_root root;
};

struct cf_map_iter {
    struct rb_node* node;
};

cf_map_t* cf_map_new() {
    cf_map_t* m = cf_malloc_z(sizeof(cf_map_t));
    return m;
}
void cf_map_delete(cf_map_t* self) {
    if (self) {
        struct rb_node* node = rb_first(&self->root);
        while (node) {
             map_item* item = CF_TYPE_CAST(map_item*, node);
            if (item->value) {
                // TODO: delte value
            }
            node = rb_next(node);
        }
        cf_free(self);
    }
}

cf_size_t cf_map_size(cf_map_t* self) {
    return 0;
}

void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len) {
    struct rb_node *node = self->root.rb_node; 
    cf_value_t new_key;

    cf_value_init(&new_key);
    new_key.type = CF_VALUE_CUSTOM;
    new_key.len = len;
    new_key.data.ptr = (void*)key;

    while (node) {
        map_item* data = CF_CONTAINER_OF(node, map_item, node);

        int ret = cf_value_cmp(&new_key, &data->map_key);
        if (ret < 0) {
            node = node->rb_left;
        }else if (ret > 0) {
            node = node->rb_right;
        }else {
            return data;
        }
   }
   return NULL;
}

void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value) {
    map_item *data = (map_item*)cf_malloc_z(sizeof(map_item));
    cf_value_init(&data->map_key);
    cf_value_set_data(&data->map_key, (void*)key, len);

    data->value = value;
    
    struct rb_node **new_node = &(self->root.rb_node), *parent = NULL;
    while (*new_node) {
        map_item *this_node = CF_CONTAINER_OF(*new_node, map_item, node);
        int ret = cf_value_cmp(&data->map_key, &this_node->map_key);
        parent = *new_node;

        if (ret < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (ret > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            this_node->value = value;
            cf_value_deinit(&data->map_key);
            cf_free(data);
            return;
        }
    }

    rb_link_node(&data->node, parent, new_node);
    rb_insert_color(&data->node, &self->root);
}

cf_map_iter_t cf_map_iter_init(cf_map_t* self) {
    struct cf_map_iter it;
    it.node = rb_first(&self->root);
    return it;
}

cf_map_iter_t cf_map_iter_next(cf_map_iter_t it) {
    struct cf_map_iter it_next;
    it_next.node = rb_next(it.node);
    return it_next;
}

cf_bool_t cf_map_iter_end(cf_map_iter_t it) {
    return it.node == CF_NULL_PTR;
}

void* cf_map_iter_value(cf_map_iter_t it) {
     map_item* item = CF_TYPE_CAST(map_item*, it.node);
    return item->value;
}

