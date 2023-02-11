/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    vector.h
 * @brief   vector implimentation.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2022-8-7
 ****************************************************/
#ifndef __CFAST_VECTOR_H__
#define __CFAST_VECTOR_H__

#include "cf/types.h"

/**
 * @defgroup CF_VECTOR vector
 * an cpp-like vector
 * @{
 */

typedef struct {
    void*           buffer;
    cf_size_t       capacity;
    cf_size_t       elm_size;
    cf_size_t       elm_count;
    void*           front;
    void*           back;
} cf_vector_t;


cf_bool_t cf_vector_init(cf_vector_t* self, size_t elm_size);
void cf_vector_deinit(cf_vector_t* self);
void cf_vector_reserve(cf_vector_t* vec, size_t cnt);
cf_bool_t cf_vector_push_back(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_push_front(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_pop_back(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_pop_front(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_fetch_back(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_fetch_front(cf_vector_t* vec, void* data, size_t size);
cf_bool_t cf_vector_at(cf_vector_t* vec, int idx, void* data, size_t size);
void* cf_vector_nextof(cf_vector_t* vec, void* data);
cf_bool_t cf_vector_resize(cf_vector_t* vec, size_t new_size, void* data);
void cf_vector_clear(cf_vector_t* vec);
static inline cf_size_t cf_vector_size(cf_vector_t* vec) { return vec->elm_count; }
static inline cf_bool_t cf_vector_is_empty(cf_vector_t* vec) { return vec->elm_count == 0; }
static inline void* cf_vector_front(cf_vector_t* vec) { return vec->front; }
static inline void* cf_vector_back(cf_vector_t* vec) { return vec->back; }

#define CF_VECTOR_DEFINE_PUSH_BACK_FUNC(t)                              \
static inline cf_bool_t cf_vector_push_back_##t(cf_vector_t* vec, t v) { \
    return cf_vector_push_back(vec, &v, sizeof(t));                    \
}

#define CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(t)                             \
static inline cf_bool_t cf_vector_push_front_##t(cf_vector_t* vec, t v) {\
    return cf_vector_push_front(vec, &v, sizeof(t));                   \
}

#define CF_VECTOR_DEFINE_POP_BACK_FUNC(t)                               \
static inline t cf_vector_pop_back_##t(cf_vector_t* vec) {              \
    t v;                                                                \
    cf_vector_pop_back(vec, &v, sizeof(v));                            \
    return v;                                                           \
}

#define CF_VECTOR_DEFINE_POP_FRONT_FUNC(t)                              \
static inline t cf_vector_pop_front_##t(cf_vector_t* vec) {             \
    t v;                                                                \
    cf_vector_pop_front(vec, &v, sizeof(t));                           \
    return v;                                                           \
}

CF_VECTOR_DEFINE_PUSH_BACK_FUNC(char)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(int)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_int8_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_uint8_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_int16_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_uint16_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_int32_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_uint32_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_int64_t)
CF_VECTOR_DEFINE_PUSH_BACK_FUNC(cf_uint64_t)

CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(char)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(int)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_int8_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_uint8_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_int16_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_uint16_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_int32_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_uint32_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_int64_t)
CF_VECTOR_DEFINE_PUSH_FRONT_FUNC(cf_uint64_t)

CF_VECTOR_DEFINE_POP_BACK_FUNC(char)
CF_VECTOR_DEFINE_POP_BACK_FUNC(int)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_int8_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_uint8_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_int16_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_uint16_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_int32_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_uint32_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_int64_t)
CF_VECTOR_DEFINE_POP_BACK_FUNC(cf_uint64_t)

CF_VECTOR_DEFINE_POP_FRONT_FUNC(char)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(int)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_int8_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_uint8_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_int16_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_uint16_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_int32_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_uint32_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_int64_t)
CF_VECTOR_DEFINE_POP_FRONT_FUNC(cf_uint64_t)

/** @} */

#endif /* __CFAST_VECTOR_H__ */
