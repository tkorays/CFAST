#include <cfast.h>

int main() {
    cf_char_t s[100];

    printf("CPU core count: %d\n", cf_sys_cpu_core_count());
    printf("CPU thread count: %d\n", cf_sys_cpu_thread_count());
    printf("CPU freq: %dMHz\n", cf_sys_cpu_freq_mhz());
    printf("Memory size: %dM\n", cf_sys_mem_size_m());
    cf_sys_cpu_brand(s, 100);
    printf("CPU brand: %s\n", s);
    cf_sys_hostname(s, 100);
    printf("Host name: %s\n", s);
    return 0;
}