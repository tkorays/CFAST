#include <cf/queue.h>
#include <cf/err.h>
#include <cf/memory.h>

typedef struct {
    cf_size_t   size;
    cf_void_t*  data;
} cf_que_item_t;

typedef struct cf_que_s {
    cf_size_t       max_size;
    cf_que_item_t*  items;
    cf_uint_t       head;
    cf_uint_t       tail;
    cf_bool_t       empty;
} cf_que_t;


cf_que_t* cf_que_create(cf_size_t max_size) {
    cf_que_t* que = CF_NULL_PTR;
    if(max_size == 0) return CF_NULL_PTR;

    que = (cf_que_t*)cf_malloc(sizeof(cf_que_t));
    if(!que) return CF_NULL_PTR;

    que->items = (cf_que_item_t*)cf_malloc(sizeof(cf_que_item_t)*max_size);
    if(!que->items) {
        cf_free(que);
        return CF_NULL_PTR;
    }

    que->max_size = max_size;
    que->head = 0;
    que->tail = 0;
    que->empty = CF_TRUE;
    return que;
}

cf_errno_t  cf_que_destroy(cf_que_t* que) {
    if(!que) return CF_EPARAM;
    if(que->items) cf_free(que->items);
    cf_free(que);
    return CF_OK;
}

cf_errno_t  cf_que_enqueue(cf_que_t* que, cf_void_t* data, cf_size_t size) {
    if(!que || !data || size == 0) return CF_EPARAM;
    /* full */
    if(que->empty == CF_FALSE && que->tail == que->head) return CF_EFULL;

    que->items[que->tail].size = size;
    que->items[que->tail].data = data;

    que->tail = (que->tail + 1) % que->max_size;

    if(que->empty == CF_TRUE) que->empty = CF_FALSE;
    return CF_OK;
}
cf_errno_t  cf_que_dequeue(cf_que_t* que, cf_void_t** data, cf_size_t* size) {
    if(!que || !data || size == 0) return CF_EPARAM;
    if(que->empty == CF_TRUE) return CF_EEMPTY;

    *data = que->items[que->head].data;
    *size = que->items[que->head].size;
    que->head = (que->head + 1) % que->max_size;

    if(que->head == que->tail) que->empty = CF_TRUE;
    return CF_OK;
}

cf_bool_t cf_lite_queue_init(cf_lite_queue_t* self, cf_size_t elm_size, cf_size_t init_capacity) {
    if (elm_size == 0 || elm_size > CF_LITE_QUEUE_MAX_ITEM_SIZE) {
        return CF_FALSE;
    }
    if (init_capacity == 0) {
        init_capacity = CF_LITE_QUEUE_INIT_CAPACITY;
    }

    self->elm_size     = elm_size;
    self->capacity      = CF_MIN2(init_capacity, CF_LITE_QUEUE_MAX_CAPACITY);
    self->head          = 0;
    self->tail          = 0;
    self->count         = 0;
    self->items         = cf_malloc(self->elm_size * self->capacity);
    return CF_TRUE;
}

void cf_lite_queue_deinit(cf_lite_queue_t* self) {
    if (self->items) {
        cf_free(self->items);
    }
    self->elm_size      = 0;
    self->capacity      = 0;
    self->head          = 0;
    self->tail          = 0;
    self->count         = 0;
    self->items         = CF_NULL_PTR;
}

void* cf_lite_queue_front(cf_lite_queue_t* self) {
    if (cf_lite_queue_empty(self)) {
        return CF_NULL_PTR;
    }

    return CF_ARRAY_GET(void*, self->items, self->elm_size, self->head);
}

void* cf_lite_queue_back(cf_lite_queue_t* self) {
        if (cf_lite_queue_empty(self)) {
        return CF_NULL_PTR;
    }

    return CF_ARRAY_GET(void*, self->items, self->elm_size, self->tail);
}

cf_bool_t cf_lite_queue_extend(cf_lite_queue_t* self) {
    size_t new_capacity = self->capacity * 2;
    void* new_items = cf_malloc(new_capacity * self->elm_size);
    cf_uint_t partial_count = 0;
    if (CF_NULL_PTR == new_items) {
        return CF_FALSE;
    }

    if (self->tail > self->head) {
        cf_memcpy_s(new_items, new_capacity * self->elm_size,\
            &((cf_uint8_t*)(self->items))[self->elm_size*self->head],\
            self->count*self->elm_size);
    } else {
        partial_count = self->capacity - self->head;
        cf_memcpy_s(new_items, new_capacity * self->elm_size,\
            &((cf_uint8_t*)(self->items))[self->elm_size*self->head],\
            partial_count*self->elm_size);
        cf_memcpy_s((cf_uint8_t*)new_items + partial_count * self->elm_size,\
            (new_capacity - partial_count) * self->elm_size,\
            self->items,
            (self->tail + 1)*self->elm_size);
    }
    cf_free(self->items);
    self->capacity = new_capacity;
    self->items = new_items;
    self->head = 0;
    self->tail = self->count - 1;
    return CF_TRUE;
}

cf_bool_t cf_lite_queue_push_back(cf_lite_queue_t* self, void* data) {
    if (!data) {
        return CF_FALSE;
    }

    if (self->count == self->capacity) {
        if (!cf_lite_queue_extend(self)) {
            return CF_FALSE;
        }
    }

    if (self->count == 0) {
        self->head = 0;
        self->tail = 0;
    } else {
        self->tail = (self->tail + 1) % self->capacity;
    }
    cf_memcpy_s((void*)&((((cf_uint8_t*)(self->items))[self->elm_size * self->tail])), \
        self->elm_size, data, self->elm_size);
    self->count++;
    return CF_TRUE;
}

cf_bool_t cf_lite_queue_push_front(cf_lite_queue_t* self, void* data) {
    if (!data) {
        return CF_FALSE;
    }

    if (self->count == self->capacity) {
        if (!cf_lite_queue_extend(self)) {
            return CF_FALSE;
        }
    }

    if (self->count == 0) {
        self->head = 0;
        self->tail = 0;
    } else {
        self->head = (self->capacity + self->head - 1) % self->capacity;
    }
    cf_memcpy_s((void*)&((((cf_uint8_t*)(self->items))[self->elm_size * self->head])), \
        self->elm_size, data, self->elm_size);
    self->count++;
    return CF_TRUE;
}

void* cf_lite_queue_pop_front(cf_lite_queue_t* self) {
    void* data = CF_NULL_PTR;
    if (cf_lite_queue_empty(self)) {
        return CF_NULL_PTR;
    }
    data = cf_lite_queue_front(self);
    self->count--;
    if (self->count == 0) {
        self->head = 0;
        self->tail = 0;
    } else {
        self->head = (self->head + 1) % self->capacity;
    }
    return data;
}

void* cf_lite_queue_pop_back(cf_lite_queue_t* self) {
    void* data = CF_NULL_PTR;
    if (cf_lite_queue_empty(self)) {
        return CF_NULL_PTR;
    }
    data = cf_lite_queue_back(self);
    self->tail = (self->capacity + self->head - 1) % self->capacity;
    self->count--;
    return data;
}

void cf_lite_queue_clear(cf_lite_queue_t* self) {
    self->head = 0;
    self->tail = 0;
    self->count = 0;
}