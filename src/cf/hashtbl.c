#include "cf/hashtbl.h"
#include "cf/memory.h"


typedef struct hashtbl_node {
    struct hashtbl_node*    next;
    cf_uint32_t             hash;   /** hash index */
    void*                   value;  /** hash value */
} hashtbl_node_t;


struct cf_hashtbl {
    hashtbl_node_t*         table;
    cf_size_t               size;
    cf_size_t               capacity;
};


cf_uint32_t cf_hash_calc(cf_uint32_t init_val, cf_void_t* key, cf_size_t len) {
    const cf_uint8_t* p = CF_TYPE_CAST(const cf_uint8_t*, key);
    const cf_uint8_t* end = CF_NULL_PTR;

    if (len == CF_HASH_KEY_LEN_AUTO) {
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
    tbl->table = cf_malloc(sizeof(hashtbl_node_t) * (init_size + 1));
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

cf_void_t* cf_hashtbl_get(cf_hashtbl_t* self, cf_uint32_t key) {

}

cf_bool_t cf_hashtbl_set(cf_hashtbl_t* self, cf_uint32_t, cf_void_t* value) {

}

