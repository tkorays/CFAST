#include <gtest/gtest.h>
#include <cf/time.h>

TEST(cf_time_test, test) {
    cf_datetime_t dt;
    ASSERT_EQ(CF_TRUE, cf_datetime_now(&dt));
    ASSERT_EQ(CF_TRUE, dt.utc);
    printf("year: %d month: %d day: %d hour: %d minute: %d second: %d millisecond: %d wday: %d timestamp: %lld\n",
        dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.millisecond, dt.week_day, dt.timestamp);
}