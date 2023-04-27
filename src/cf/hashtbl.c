#include "cf/hashtbl.h"
#include "cf/memory.h"
#include <string.h>


/**
 * use a linked list to store values with the same hash value.
 */
typedef struct hashtbl_node {
    struct hashtbl_node*    next;
    cf_uint32_t             hash;   /** hash index */
    cf_void_t*              key;    /** original key */
    cf_size_t               keylen; /** bytes of original key */
    void*                   value;  /** hash value */
} hashtbl_node_t;


/**
 * definition of hash table.
 */
struct cf_hashtbl {
    /**
     * Use a list (table) to store unique hashs.
     * Use a linked list (hashtbl_node_t) to store collision keys.
     * `table` is a pointer list points to node lists.
     */
    hashtbl_node_t**        table;

    /** track the item count */
    cf_size_t               size;

    /** hash mask is used to convert uint32 hash to a limited value. */
    cf_uint32_t             hashmsk;
};


cf_void_t hashtbl_destroy_node(hashtbl_node_t* node) {
    hashtbl_node_t* p = node;
    while (p) {
        node = p->next;
        if(p->key) {
            cf_free(p->key);
        }
        /** callback for destroy value */
        cf_free(node);
        p = node;
    }
}


/**
 * find or create hash node from a uint32 hash key.
 */
hashtbl_node_t* hashtbl_get_node_by_hash(cf_hashtbl_t* tbl, cf_uint32_t hash, cf_bool_t try_new) {
    hashtbl_node_t* node = CF_NULL_PTR, **list_entry = CF_NULL_PTR;
    hash &= tbl->hashmsk;
    list_entry = &tbl->table[hash]; /* point to list */
    if (*list_entry) {
        return *list_entry;
    }

    node = cf_malloc_z(sizeof(hashtbl_node_t));
    node->hash = hash; 

    *list_entry = node;
    tbl->size++;
    return node;
}

hashtbl_node_t* hashtbl_get_node(cf_hashtbl_t* tbl, const cf_void_t* key, cf_size_t len) {
    cf_uint32_t hash = 0;
    hashtbl_node_t* node = CF_NULL_PTR, **list_entry = CF_NULL_PTR;
    const cf_uint8_t* p = CF_TYPE_CAST(const cf_uint8_t*, key);
    const cf_uint8_t* end = CF_NULL_PTR;

    /* get the hash from key */
    if (len == CF_HASH_STRING_KEY_LEN_AUTO) {
        len = 0;
        // determin string length automaticlly.
        while (*p != '\0') {
            hash = (hash * 33) + *p;
            ++p;
            ++len;
        }
        len++;
    } else {
        end = p + len;
        for (; p != end; p++) {
            hash = (hash* 33) + *p;
        } 
    }
    hash &= tbl->hashmsk;

    for (list_entry = &tbl->table[hash], node = *list_entry;
         node;
         list_entry = &node->next, node = *list_entry) {
        if (node->hash == hash && len == node->keylen
            && node->key && memcmp(node->key, key, len) == 0) {
            return node;
        }
    }
    node = cf_malloc_z(sizeof(hashtbl_node_t));
    node->hash = hash; 
    node->key = cf_malloc(len);
    node->keylen = len;
    cf_memcpy_s(node->key, len, key, len);

    *list_entry = node;
    tbl->size++;
    return node;
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
    
    tbl->hashmsk = init_size;
    tbl->size = 0;
    tbl->table = cf_malloc_z(sizeof(hashtbl_node_t*) * (init_size + 1));
    if (!tbl->table) {
        cf_free(tbl);
        return CF_NULL_PTR;
    }
    return tbl;
}

cf_void_t cf_hashtbl_delete(cf_hashtbl_t* self, cf_hashtbl_cb_f cb) {
    hashtbl_node_t* p, *n;
    int i;
    if (CF_NULL_PTR == self) return ;

    /* destroy table items and apply callback */
    for (i = 0; i < self->hashmsk + 1; i++) {
        n = self->table[i]; 
        if (!n) continue;

        p = n;
        while (p) {
            n = p->next;
            if(p->key) {
                cf_free(p->key);
            }
            if (cb) {
                cb(p->value);
            }
            cf_free(p);
            p = n;
        }
    } 

    if (self->table) {
        cf_free(self->table);
    }
    cf_free(self);
}

cf_void_t* cf_hashtbl_get_by_hash(cf_hashtbl_t* self, cf_uint32_t hash) {
    hashtbl_node_t* node = CF_NULL_PTR;
    node = hashtbl_get_node_by_hash(self, hash, CF_FALSE);
    return node ? node->value : CF_NULL_PTR;
}

cf_void_t cf_hashtbl_set_by_hash(cf_hashtbl_t* self, cf_uint32_t hash, cf_void_t* value) {
    hashtbl_node_t* node = CF_NULL_PTR;
    cf_bool_t create = value ? CF_TRUE : CF_FALSE;
    node = hashtbl_get_node_by_hash(self, hash, create);
    if (node) {
        if (CF_NULL_PTR == value) {
            node = node->next;
            self->size--;
        } else {
            node->value = value;
        }
    }
}


cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, const cf_void_t* key, cf_size_t len) {
    hashtbl_node_t* node = CF_NULL_PTR;
    node = hashtbl_get_node(self, key, len);
    return node ? node->value : CF_NULL_PTR;
}

cf_void_t cf_hashtbl_set(cf_hashtbl_t* self, const cf_void_t* key, cf_size_t len, cf_void_t* value) {
    hashtbl_node_t* node = CF_NULL_PTR;
    cf_bool_t create = value ? CF_TRUE : CF_FALSE;
    node = hashtbl_get_node(self, key, len);
    if (node) {
        if (CF_NULL_PTR == value) {
            node = node->next;
            self->size--;
        } else {
            node->value = value;
        }
    }
}

cf_size_t cf_hashtbl_size(cf_hashtbl_t* self) {
    return self->size;
}

cf_hashtbl_iter_t cf_hashtbl_iter_init(cf_hashtbl_t* self) {
    cf_uint32_t i;
    /* find the first table index */
    for (i = 0; i <= self->hashmsk; i++) {
        if (self->table[i]) {
            return self->table[i];
        }
    }
    return CF_NULL_PTR;
}

cf_hashtbl_iter_t cf_hashtbl_iter_next(cf_hashtbl_t* self, cf_hashtbl_iter_t it) {
    /* the next index */
    cf_uint32_t i = it->hash + 1;
    if (it->next) {
        return it->next;
    }
    while (i <= self->hashmsk) {
        if (self->table[i]) {
            return self->table[i];
        }
        i++;
    }
    return CF_NULL_PTR;
}

cf_uint32_t cf_hashtbl_iter_hash(cf_hashtbl_iter_t it) {
    return it->hash;
}

cf_void_t* cf_hashtbl_iter_key(cf_hashtbl_iter_t it) {
    return it->key;
}

cf_void_t* cf_hashtbl_iter_value(cf_hashtbl_iter_t it) {
    return it->value;
}

