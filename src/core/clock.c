#include "cf/clock.h"
#include "cf/memory.h"

static cf_clock_t* _g_clock_global_ = CF_NULL_PTR;
static cf_clock_t* _g_clock_steady_ = CF_NULL_PTR;
static cf_clock_t* _g_clock_system_ = CF_NULL_PTR;

cf_clock_t* cf_clock_global() {
    if (CF_NULL_PTR == _g_clock_global_) {
        _g_clock_global_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_global_);
    }
    return _g_clock_global_;
}

void cf_clock_set_global(cf_clock_t* clock) {
    /* TODO: reset when not null */
    if (CF_NULL_PTR == _g_clock_global_) {
        _g_clock_global_ = clock;
    }
}

cf_clock_t* cf_clock_steady() {
    if (CF_NULL_PTR == _g_clock_steady_) {
        _g_clock_steady_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_steady_);
    }
    return _g_clock_steady_;
}

cf_clock_t* cf_clock_system() {
    if (CF_NULL_PTR == _g_clock_system_) {
        _g_clock_system_ = CF_TYPE_CAST(cf_clock_t*, cf_malloc(sizeof(cf_clock_t)));
        cf_clock_init(_g_clock_system_);
    }
    return _g_clock_system_;
}

