#include "cf/hashtbl.h"
#include "cf/memory.h"


typedef struct hashtbl_node {
    struct hashtbl_node*    next;
    cf_uint32_t             hash;   /** hash index */
    void*                   value;  /** hash value */
} hashtbl_node_t;


struct cf_hashtbl {
    hashtbl_node_t**        table;
    cf_size_t               size;
    cf_size_t               capacity;
};

cf_uint32_t cf_hash_calc(cf_uint32_t init_val, cf_void_t* key, cf_size_t len);
hashtbl_node_t* hashtbl_get_node(cf_hashtbl_t* tbl, cf_uint32_t hash, cf_bool_t try_new);


cf_uint32_t cf_hash_calc(cf_uint32_t init_val, cf_void_t* key, cf_size_t len) {
    const cf_uint8_t* p = CF_TYPE_CAST(const cf_uint8_t*, key);
    const cf_uint8_t* end = CF_NULL_PTR;

    if (len == CF_HASH_STRING_KEY_LEN) {
        while (*p) {
            init_val = (init_val * 33) + *p;
            ++p;
        }
    } else {
        end = p + len;
        for (; p != end; p++) {
            init_val = (init_val * 33) + *p;
        } 
    }
    return init_val;
}


cf_hashtbl_t* cf_hashtbl_new(cf_size_t size) {
    cf_size_t init_size = 8;
    cf_hashtbl_t* tbl = CF_NULL_PTR;

    tbl = cf_malloc_z(sizeof(cf_hashtbl_t));
    if (!tbl) {
        return CF_NULL_PTR;
    }

    do { init_size <<= 1; } while (init_size < size);
    init_size -= 1;
    
    tbl->capacity = init_size;
    tbl->size = 0;
    tbl->table = cf_malloc(sizeof(hashtbl_node_t*) * (init_size + 1));
    if (!tbl->table) {
        cf_free(tbl);
        return CF_NULL_PTR;
    }
    return tbl;
}

cf_void_t cf_hashtbl_delete(cf_hashtbl_t* self) {
    if (self != CF_NULL_PTR) {
        if (self->table) {
            cf_free(self->table);
        }
        cf_free(self);
    }
}

cf_void_t* cf_hashtbl_get_by_hash(cf_hashtbl_t* self, cf_uint32_t hash) {
    hashtbl_node_t* node = CF_NULL_PTR;
    node = hashtbl_get_node(self, hash, CF_FALSE);
    return node ? node->value : CF_NULL_PTR;
}

cf_void_t cf_hashtbl_set_by_hash(cf_hashtbl_t* self, cf_uint32_t hash, cf_void_t* value) {
    hashtbl_node_t* node = CF_NULL_PTR;
    cf_bool_t create = value ? CF_TRUE : CF_FALSE;
    node = hashtbl_get_node(self, hash, create);
    if (node) {
        if (CF_NULL_PTR == value) {
            node = node->next;
            self->size--;
        } else {
            node->value = value;
        }
    }
}

hashtbl_node_t* hashtbl_get_node(cf_hashtbl_t* tbl, cf_uint32_t hash, cf_bool_t try_new) {
    hashtbl_node_t* node = CF_NULL_PTR, **list_entry = CF_NULL_PTR;
    for (list_entry = &tbl->table[hash], node = *list_entry;
         node;
         list_entry = &node->next, node = *list_entry) {
        if (node->hash == hash) {
            return node;
        }
    }

    node = cf_malloc_z(sizeof(hashtbl_node_t));
    node->hash = hash; 

    *list_entry = node;
    tbl->size++;
    return node;
}

cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len) {
    return CF_NULL_PTR;
}

cf_void_t cf_hashtbl_set(cf_hashtbl_t* self, cf_void_t* key, cf_size_t len, cf_void_t* value) {

}
