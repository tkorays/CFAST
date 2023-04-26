#include "cf/clock.h"
#include "cf/memory.h"

#include <time.h>
#ifdef CF_OS_WIN
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#ifdef CF_OS_WIN
static LONGLONG _G_WIN_QPC_FREQ_ = 0;
#endif

static cf_clock_t* _g_clock_global_ = CF_NULL_PTR;
static cf_clock_t* _g_clock_steady_ = CF_NULL_PTR;
static cf_clock_t* _g_clock_steady_high_ = CF_NULL_PTR;
static cf_clock_t* _g_clock_system_ = CF_NULL_PTR;

cf_clock_t* cf_clock_global() {
    if (CF_NULL_PTR == _g_clock_global_) {
        _g_clock_global_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_global_);
        _g_clock_global_->steady_flag = 1;
    }
    return _g_clock_global_;
}

void cf_clock_set_global(cf_clock_t* clock) {
    _g_clock_global_ = clock;
}

cf_clock_t* cf_clock_steady() {
    if (CF_NULL_PTR == _g_clock_steady_) {
        _g_clock_steady_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_steady_);
        _g_clock_steady_->steady_flag = 1;
    }
    return _g_clock_steady_;
}

cf_clock_t* cf_clock_steady_high() {
    if (CF_NULL_PTR == _g_clock_steady_high_) {
        _g_clock_steady_high_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_steady_high_);
        _g_clock_steady_high_->steady_high_flag = 1;
    }
    return _g_clock_steady_;
}

cf_clock_t* cf_clock_system() {
    if (CF_NULL_PTR == _g_clock_system_) {
        _g_clock_system_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_system_);
        _g_clock_system_->system_flag = 1;
    }
    return _g_clock_system_;
}

void cf_clock_init(cf_clock_t* self) {
    cf_membzero(self, sizeof(cf_clock_t));
    return;
}

void cf_clock_deinit(cf_clock_t* self) {
    return;
}

cf_int64_t cf_clock_current_ns(cf_clock_t* self) {
#ifdef CF_OS_WIN
    // https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
    LARGE_INTEGER clock_result, freq_result;
    ULONGLONG tick_ms;
    if (self->steady_high_flag) {
        if (_G_WIN_QPC_FREQ_ == 0) {
            if (!QueryPerformanceFrequency(&freq_result)) {
                return -1;
            }
            _G_WIN_QPC_FREQ_ = freq_result.QuadPart;
        }
        
        if (!QueryPerformanceCounter(&clock_result)) {
            return -1;
        }
        return 1000000000 * CF_TYPE_CAST(cf_int64_t, clock_result.QuadPart / _G_WIN_QPC_FREQ_);
    } else if (self->steady_flag) {
        tick_ms = GetTickCount64();
        return 1000000 * CF_TYPE_CAST(cf_int64_t, tick_ms);
    } else {
        return -1;
    }
#else
    struct timespec ts;
    if (self->steady_high_flag || self->steady_flag) {
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
            return -1;
        }
    } else {
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
            return -1;
        }
    }
    return 1000000000 * CF_TYPE_CAST(cf_int64_t, ts.tv_sec) + ts.tv_nsec;
#endif
}

cf_int64_t cf_clock_current_ms(cf_clock_t* self) {
    cf_int64_t ret = cf_clock_current_ns(self);
    return ret < 0 ? -1 : ret / 1000000;
}

cf_int64_t cf_clock_current_s(cf_clock_t* self) {
    cf_int64_t ret = cf_clock_current_ns(self);
    return ret < 0 ? -1 : ret / 1000000000;
}
