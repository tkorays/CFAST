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

cf_void_t cf_time_sleep(cf_uint32_t ms)
{
#ifdef CF_OS_WIN
    Sleep(ms);
#else 
    usleep(ms*1000);
#endif
}

cf_bool_t cf_datetime_now(cf_datetime_t* dt) {
#ifdef CF_OS_WIN
    SYSTEMTIME st;
    GetLocalTime(&st);
    dt->year = st.wYear;
    dt->month = st.wMonth;
    dt->day = st.wDay;
    dt->hour = st.wHour;
    dt->minute = CF_TYPE_CAST(cf_uint8_t, st.wMinute);
    dt->second = CF_TYPE_CAST(cf_uint8_t, st.wSecond);
    dt->millisecond = st.wMilliseconds;
    dt->week_day = st.wDayOfWeek;
    dt->utc = CF_TRUE;
#else
    struct tm* t;
    time_t timer;
    struct timeval tv;
    if (!ti) return CF_FALSE;
    (void)gettimeofday(&tv, NULL);

    timer = time(NULL);
    t = localtime(&timer);

    dt->year = t->tm_year + 1900;
    dt->month = t->tm_mon + 1;
    dt->day = t->tm_mday;
    dt->hour = t->tm_hour;
    dt->minute = t->tm_min;
    dt->second = t->tm_sec;
    dt->millisecond = tv.tv_usec / 1000;
    dt->week_day = t->tm_wday;
    dt->utc = CF_TRUE;
#endif
    return CF_TRUE;
}

cf_int32_t cf_datetime_day_of_year(cf_datetime_t* dt) {
    cf_int32_t d = 0;
    cf_uint32_t i;
    cf_bool_t leap_year = CF_FALSE;
    leap_year = CF_IS_LEAP_YEAR(dt->year);
    for (i = 0; i < dt->month; i++) {
        d += g_mondays[i];
    }
    if (dt->month > 2 && leap_year) d++;
    d += dt->day;
    return d;
}
