#include <cfast.h>
#include <stdio.h>


int main() {
    cf_int_t i;
    cf_timeinfo_t ti;
    cf_char_t s[100] = {0};
    cf_clock_t t1 = 0, t2 = 0;
    cf_time_now(&ti);
    printf("%d-%d-%d %d:%d:%d\n", ti.year, ti.mon, ti.mday, ti.hour, ti.min, ti.sec);
    cf_time_str(&ti, s, 100);
    printf("%s\n", s);
    cf_time_strYMDHMS(&ti, s, 100);
    printf("%s\n", s);
    printf("us: %d\n", ti.e_usec);

    t1 = cf_clock_get();
    for(i = 0; i < 100; i++) {
        cf_time_sleep(10);
        cf_time_now(&ti);
        cf_time_strYMDHMS(&ti, s, 100);
        printf("%s\n", s);
    }
    t2 = cf_clock_get();
    printf(">>>%d\n", cf_clock_diff_ms(t2, t1));
    return 0;
}