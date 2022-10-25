#include "cf/time.h"
#include "cf/assert.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    cf_datetime_t dt;
    cf_assert(CF_TRUE == cf_datetime_now(&dt));
    cf_assert(CF_TRUE == dt.utc);
    printf("year: %d month: %d day: %d hour: %d minute: %d second: %d millisecond: %d wday: %d timestamp: %lld\n",
        dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.millisecond, dt.week_day, dt.timestamp);
    return 0;
}
