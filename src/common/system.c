#include <cf/system.h>
#ifdef CF_OS_WIN

#elif CF_OS_MAC

#include <sys/sysctl.h>
#include <mach/machine.h>
#else 

#endif

CF_DECLARE(cf_cpu_type_t) cf_sys_cpu_type() {
    cf_int_t cputype;
    size_t size;
    sysctlbyname("hw.cputype", &cputype, &size, NULL, 0);
    switch(cputype) {
    case CPU_TYPE_X86: return CF_CPU_TYPE_X86;
    case CPU_TYPE_ARM: return CF_CPU_TYPE_ARM;
    case CPU_TYPE_POWERPC: return CF_CPU_TYPE_POWERPC;
    default: return CF_CPU_TYPE_OTHER;
    }
}
CF_DECLARE(cf_cpu_arch_t) cf_sys_cpu_arch() {
    return 0;
}

CF_DECLARE(cf_int_t) cf_sys_cpu_core_count() {
    int count = 0;
    size_t size = sizeof(int);
    sysctlbyname("machdep.cpu.core_count", &count, &size, NULL, 0);
    return count;
}

CF_DECLARE(cf_int_t) cf_sys_cpu_thread_count() {
    int count = 0;
    size_t size = sizeof(int);
    sysctlbyname("machdep.cpu.thread_count", &count, &size, NULL, 0);
    return count;
}

CF_DECLARE(cf_int_t) cf_sys_cpu_freq_mhz() {
    cf_uint64_t freq = 0;
    size_t size = sizeof(cf_uint64_t);
    sysctlbyname("hw.cpufrequency", &freq, &size, NULL, 0);
    return (cf_int_t)(freq /1000000);
}

CF_DECLARE(cf_void_t) cf_sys_cpu_brand(cf_char_t* buf, cf_size_t size) {
    if(!buf) return ;
    sysctlbyname("machdep.cpu.brand_string", buf, &size, NULL, 0);
}

CF_DECLARE(cf_int_t) cf_sys_mem_size_m() {
    cf_uint64_t mem;
    size_t size = sizeof(cf_uint64_t);
    sysctlbyname("hw.memsize", &mem, &size, NULL, 0);
    return (cf_int_t)(mem >> 20);
}

CF_DECLARE(cf_void_t) cf_sys_hostname(cf_char_t* buf, cf_size_t size) {
    if(!buf) return;
    sysctlbyname("kern.hostname", buf, &size, NULL, 0);
}
