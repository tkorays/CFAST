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
    cf_value_cmp_f map_key_cmp;
};


cf_map_t* cf_map_new(cf_value_cmp_f f) {
    cf_map_t* m = cf_malloc_z(sizeof(cf_map_t));
    m->map_key_cmp = f;
    if (m->map_key_cmp == CF_NULL_PTR) {
        m->map_key_cmp = cf_value_cmp;
    }
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

void* __cf_map_get_internal(cf_map_t* self, cf_value_t* map_key) {
    struct rb_node *node = self->root.rb_node; 
    while (node) {
        map_item* data = CF_CONTAINER_OF(node, map_item, node);

        int ret = self->map_key_cmp(map_key, &data->map_key);
        if (ret < 0) {
            node = node->rb_left;
        }else if (ret > 0) {
            node = node->rb_right;
        }else {
            return data->value;
        }
   }
   return NULL;
}

void __cf_map_set_internal(cf_map_t* self, cf_value_t* map_key, cf_void_t* value) {
    map_item *data = CF_NULL_PTR;
    
    struct rb_node **new_node = &(self->root.rb_node), *parent = NULL;
    while (*new_node) {
        map_item *this_node = CF_CONTAINER_OF(*new_node, map_item, node);
        int ret = self->map_key_cmp(map_key, &this_node->map_key);
        parent = *new_node;

        if (ret < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (ret > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            this_node->value = value;
            return;
        }
    }

    data = (map_item*)cf_malloc_z(sizeof(map_item));
    cf_memcpy_s(&data->map_key, sizeof(cf_value_t), map_key, sizeof(cf_value_t));
    cf_membzero(map_key, sizeof(cf_value_t));
    data->value = value;
    rb_link_node(&data->node, parent, new_node);
    rb_insert_color(&data->node, &self->root);
    return;
}

void* cf_map_get(cf_map_t* self, const cf_void_t* key, cf_size_t len) {
    cf_value_t map_key;

    cf_value_init(&map_key);
    map_key.type = CF_VALUE_CUSTOM;
    map_key.len = len;
    map_key.data.ptr = (void*)key;

    return __cf_map_get_internal(self, &map_key);
}

void cf_map_set(cf_map_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value) {
    cf_value_t map_key;
    cf_value_init(&map_key);
    cf_value_set_data(&map_key, (void*)key, len);

    __cf_map_set_internal(self, &map_key, value);
    cf_value_deinit(&map_key);
}

cf_map_iter_t cf_map_iter_init(cf_map_t* self) {
    return (cf_map_iter_t)rb_first(&self->root);
}

cf_map_iter_t cf_map_iter_next(cf_map_iter_t it) {
    return (cf_map_iter_t)rb_next(CF_TYPE_CAST(struct rb_node*, it));
}

cf_bool_t cf_map_iter_end(cf_map_iter_t it) {
    return it == CF_NULL_PTR;
}

void* cf_map_iter_value(cf_map_iter_t it) {
     map_item* item = CF_TYPE_CAST(map_item*, CF_TYPE_CAST(struct rb_node*, it));
    return item->value;
}

