#include <cf/time.h>
#include <cf/str.h>
#include <cf/err.h>

#include <time.h>
#ifdef CF_OS_WIN
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

static char* g_month_name_abbr[] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
static char* g_weekday_name_abbr[] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
static cf_uint8_t g_mondays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


cf_clock_t cf_clock_get() {
#ifdef CF_OS_WIN
    return  (cf_clock_t)GetTickCount();
#else
    struct timeval tv;
    (void)gettimeofday(&tv, NULL);
    return (cf_clock_t)tv.tv_sec*1000 + tv.tv_usec/1000;
#endif
}

cf_uint64_t cf_clock_diff_ms(cf_clock_t next, cf_clock_t prev) {
    return (next - prev);
}

cf_void_t cf_time_sleep(cf_uint32_t ms)
{
#ifdef CF_OS_WIN
    Sleep(ms);
#else 
    usleep(ms*1000);
#endif
}

cf_errno_t cf_time_now(cf_timeinfo_t* ti)
{
#ifdef CF_OS_WIN
    SYSTEMTIME st;
    GetLocalTime(&st);
    ti->sec = st.wSecond;
    ti->min = st.wMinute;
    ti->hour = st.wHour;
    ti->mday = st.wDay;
    ti->mon = st.wMonth;
    ti->year = st.wYear;
    ti->wday = st.wDayOfWeek;
    ti->msec = st.wMilliseconds;
#else
    struct tm* t;
    time_t timer;
    struct timeval tv;
    if (!ti) return CF_EPARAM;
    (void)gettimeofday(&tv, NULL);

    timer = time(NULL);
    t = localtime(&timer);
    ti->sec = t->tm_sec;
    ti->min = t->tm_min;
    ti->hour = t->tm_hour;
    ti->mday = t->tm_mday;
    ti->mon = t->tm_mon;
    ti->year = t->tm_year;
    ti->wday = t->tm_wday;

    ti->msec = tv.tv_usec / 1000;
#endif
    
    return CF_EOK;
}

cf_errno_t cf_time_str(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size) {
    cf_int_t ret;
    if(!ti || !buf) return CF_EPARAM;
    ret = cf_snprintf(buf, size, "%s %s %d %02d:%02d:%02d %d", 
        g_weekday_name_abbr[ti->wday],
        g_month_name_abbr[ti->mon],
        ti->mday,
        ti->hour,
        ti->min,
        ti->sec,
        ti->year+1900);
    if(ret <= 0) {
        return CF_ENOK;
    }
    return CF_EOK;
}

cf_errno_t cf_time_strYMDHMS(const cf_timeinfo_t* ti, cf_char_t* buf, cf_size_t size) {
    cf_int_t ret;
    if(!ti || !buf) return CF_EPARAM;
    ret = cf_snprintf(buf, size, "%d-%d-%d %d:%d:%d.%d", 
        ti->year + 1900,
        ti->mon + 1,
        ti->mday,
        ti->hour,
        ti->min,
        ti->sec,
        ti->msec
        );
    if(ret <= 0) {
        return CF_ENOK;
    }
    return CF_EOK;
}

cf_int32_t cf_day_of_year(const cf_timeinfo_t* ti) {
    cf_int32_t d = 0;
    cf_uint32_t i;
    cf_bool_t leap_year = CF_FALSE;
    if (!ti) return -1;
    leap_year = CF_IS_LEAP_YEAR(ti->year + 1900);
    for (i = 0; i < ti->mon; i++) {
        d += g_mondays[i];
    }
    if (ti->mon > 2 && leap_year) d++;
    d += ti->mday;
    return d;
}

cf_bool_t cf_time_is_valid(const cf_timeinfo_t* ti) {
    return (ti && ti->mon < 12 && ti->mday <= 31 && ti->hour < 24
        && ti->min < 60 && ti->sec <= 60 && ti->wday < 7 && ti->msec < 1000);
}
