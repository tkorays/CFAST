#include "base/vector.h"
#include "base/memory.h"
#include "base/check.h"

#define RTC_VECTOR_BUF_DEF_BYTES 512
#define RTC_VECTOR_BUF_DEF_COUNT 128

static inline rtc_bool _rtc_vectory_copy_buffer(rtc_vector* vec, void* new_buffer, size_t new_capacity) {
    uint64_t tmp1        = 0;
    uint64_t tmp2        = 0;
    if (vec->_elm_count == 0) {
        rtc_free(vec->_buffer);
        vec->_capacity  = new_capacity;
        vec->_buffer    = new_buffer;
        vec->_front     = new_buffer;
        vec->_back      = new_buffer;
    } else {
        if ((uint64_t)(vec->_front) < (uint64_t)(vec->_back)) {
            memcpy(new_buffer, vec->_front, vec->_elm_count * vec->_elm_size);
        } else {
            tmp1 = vec->_capacity * vec->_elm_size - ((uint64_t)vec->_front - (uint64_t)vec->_buffer + vec->_elm_size);
            memcpy(new_buffer, vec->_front, tmp1);
            tmp2 = vec->_elm_count * vec->_elm_size - tmp1;
            if (tmp2 > 0) {
                memcpy((uint8_t*)new_buffer + tmp1, vec->_buffer, tmp2);
            }
        }  
        rtc_free(vec->_buffer);
        vec->_capacity  = new_capacity;
        vec->_buffer    = new_buffer;
        vec->_front     = new_buffer;
        vec->_back      = (void*)((int8_t*)new_buffer + vec->_elm_count * vec->_elm_size);
    }

    return RTC_TRUE;
}

static inline void* _rtc_vector_next_of(void* buffer, size_t elm_size, size_t capacity, void* current) {
    int offset;
    offset = (uint64_t)current - (uint64_t)buffer;
    offset = offset == (capacity - 1) * elm_size ? 0 : offset + elm_size;
    return RTC_CAST(void*, (uint8_t*)buffer + offset);
}

static inline void* _rtc_vector_prev_of(void* buffer, size_t elm_size, size_t capacity, void* current) {
    int offset;
    offset = (uint64_t)current - (uint64_t)buffer;
    offset = offset == 0 ? (capacity - 1) * elm_size : offset - elm_size;
    return RTC_CAST(void*, (uint8_t*)buffer + offset);
}

rtc_vector* rtc_vector_new(rtc_vector* vec, size_t elm_size) {
    if (elm_size == 0) return RTC_NULL;

    if (vec) {
        vec->_self_managed = RTC_FALSE;
    } else {
        vec = RTC_CAST(rtc_vector*, rtc_malloc(sizeof(rtc_vector)));
        vec->_self_managed = RTC_TRUE;
    }


    vec->_capacity  = RTC_MIN(RTC_VECTOR_BUF_DEF_BYTES, RTC_VECTOR_BUF_DEF_COUNT * elm_size) / elm_size;
    vec->_elm_size  = elm_size;
    vec->_buffer    = rtc_malloc(vec->_capacity * elm_size);
    if (!vec->_buffer) {
        if (vec->_self_managed) rtc_free(vec);
        return RTC_NULL;
    }
    vec->_elm_count = 0;
    vec->_front     = vec->_buffer;
    vec->_back      = vec->_buffer;
    return vec;
}

void rtc_vector_free(rtc_vector* vec) {
    if (vec->_self_managed) {
        rtc_free(vec);
    }
}

void rtc_vector_reserve(rtc_vector* vec, size_t cnt) {
    void* new_buffer    = RTC_NULL;
    if (cnt <= vec->_capacity) return;
    new_buffer = rtc_malloc(cnt * vec->_elm_size);
    if (!new_buffer) {
        return;
    }

    _rtc_vectory_copy_buffer(vec, new_buffer, cnt);
}

rtc_bool rtc_vector_push_back(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;

    if (vec->_capacity <= vec->_elm_count) {
        rtc_vector_reserve(vec, vec->_capacity * 2);
        if (vec->_capacity <= vec->_elm_count) {
            return RTC_FALSE;
        }
    }

    vec->_back = vec->_elm_count == 0 ? vec->_buffer :
        _rtc_vector_next_of(vec->_buffer, vec->_elm_size, vec->_capacity, vec->_back);
    memcpy(vec->_back, data, size);
    vec->_elm_count++;
    return RTC_TRUE;
}

rtc_bool rtc_vector_push_front(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;

    if (vec->_capacity <= vec->_elm_count) {
        rtc_vector_reserve(vec, vec->_capacity * 2);
        if (vec->_capacity <= vec->_elm_count) {
            return RTC_FALSE;
        }
    }
    vec->_front = vec->_elm_count == 0 ? vec->_buffer :
        _rtc_vector_prev_of(vec->_buffer, vec->_elm_size, vec->_capacity, vec->_front);
    memcpy(vec->_front, data, size);
    vec->_elm_count++;
    return RTC_TRUE;
}

rtc_bool rtc_vector_pop_back(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;

    if (rtc_vector_is_empty(vec)) return RTC_FALSE;
    memcpy(data, vec->_back, size);

    vec->_back = _rtc_vector_prev_of(vec->_buffer, vec->_elm_size, vec->_capacity, vec->_back);
    vec->_elm_count--;
    return RTC_TRUE;
}

rtc_bool rtc_vector_pop_front(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;

    if (rtc_vector_is_empty(vec)) return RTC_FALSE;
    memcpy(data, vec->_front, size);
    vec->_front = _rtc_vector_next_of(vec->_buffer, vec->_elm_count, vec->_capacity, vec->_front);
    vec->_elm_count--;
    return RTC_TRUE;
}

rtc_bool rtc_vector_back(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;
    memcpy(data, vec->_back, size);
    return RTC_TRUE;
}

rtc_bool rtc_vector_front(rtc_vector* vec, void* data, size_t size) {
    RTC_CHECK_GE(size, vec->_elm_size);
    if (!data) return RTC_FALSE;
    memcpy(data, vec->_front, size);
    return RTC_TRUE;
}

rtc_bool rtc_vector_at(rtc_vector* vec, int idx, void* data, size_t size) {
    if (!data || size != vec->_elm_size) return RTC_FALSE;
    memcpy(data, (uint8_t*)vec->_buffer, size);
    return RTC_TRUE;
}

rtc_bool rtc_vector_resize(rtc_vector* vec, size_t new_size, void* data) {
    if (new_size > vec->_capacity) {
        rtc_vector_reserve(vec, new_size);
        if (new_size > vec->_capacity) return RTC_FALSE;
    }

    while (vec->_elm_count < new_size) {
        rtc_vector_push_back(vec, data, vec->_elm_size);
    }
    return RTC_TRUE;
}

void rtc_vector_clear(rtc_vector* vec) {
    vec->_elm_count = 0;
    vec->_front     = vec->_buffer;
    vec->_back      = vec->_buffer;
}

