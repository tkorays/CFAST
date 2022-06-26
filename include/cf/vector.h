#ifndef __TEMPLE_RTC_BASE_VECTOR_H__
#define __TEMPLE_RTC_BASE_VECTOR_H__

#include "base/types.h"

typedef struct rtc_vector {
    void*       _buffer;
    size_t      _capacity;
    size_t      _elm_size;
    size_t      _elm_count;
    void*       _front;
    void*       _back;
    rtc_bool    _self_managed;
} rtc_vector;

rtc_vector* rtc_vector_new(rtc_vector* vec, size_t elm_size);
void rtc_vector_free(rtc_vector* vec);
void rtc_vector_reserve(rtc_vector* vec, size_t cnt);
rtc_bool rtc_vector_push_back(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_push_front(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_pop_back(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_pop_front(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_back(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_front(rtc_vector* vec, void* data, size_t size);
rtc_bool rtc_vector_at(rtc_vector* vec, int idx, void* data, size_t size);
rtc_bool rtc_vector_resize(rtc_vector* vec, size_t new_size, void* data);
void rtc_vector_clear(rtc_vector* vec);
static inline size_t rtc_vector_size(rtc_vector* vec) { return vec->_elm_count; }
static inline rtc_bool rtc_vector_is_empty(rtc_vector* vec) { return vec->_elm_count == 0; }

#define RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(t)                             \
static inline rtc_bool rtc_vector_push_back_##t(rtc_vector* vec, t v) { \
    return rtc_vector_push_back(vec, &v, sizeof(t));                    \
}

#define RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(t)                            \
static inline rtc_bool rtc_vector_push_front_##t(rtc_vector* vec, t v) {\
    return rtc_vector_push_front(vec, &v, sizeof(t));                   \
}

#define RTC_VECTOR_DEFINE_POP_BACK_FUNC(t)                              \
static inline t rtc_vector_pop_back_##t(rtc_vector* vec) {              \
    t v;                                                                \
    rtc_vector_pop_back(vec, &v, sizeof(v));                            \
    return v;                                                           \
}

#define RTC_VECTOR_DEFINE_POP_FRONT_FUNC(t)                             \
static inline t rtc_vector_pop_front_##t(rtc_vector* vec) {             \
    t v;                                                                \
    rtc_vector_pop_front(vec, &v, sizeof(t));                           \
    return v;                                                           \
}

RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(char)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(int)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(int8_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(uint8_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(int16_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(uint16_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(int32_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(uint32_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(int64_t)
RTC_VECTOR_DEFINE_PUSH_BACK_FUNC(uint64_t)

RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(char)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(int)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(int8_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(uint8_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(int16_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(uint16_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(int32_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(uint32_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(int64_t)
RTC_VECTOR_DEFINE_PUSH_FRONT_FUNC(uint64_t)

RTC_VECTOR_DEFINE_POP_BACK_FUNC(char)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(int)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(int8_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(uint8_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(int16_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(uint16_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(int32_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(uint32_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(int64_t)
RTC_VECTOR_DEFINE_POP_BACK_FUNC(uint64_t)

RTC_VECTOR_DEFINE_POP_FRONT_FUNC(char)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(int)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(int8_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(uint8_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(int16_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(uint16_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(int32_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(uint32_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(int64_t)
RTC_VECTOR_DEFINE_POP_FRONT_FUNC(uint64_t)

#endif // __TEMPLE_RTC_BASE_VECTOR_H__
