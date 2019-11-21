#include <cf/system.h>
#ifdef CF_OS_WIN

#elif defined(CF_OS_MAC)
#include <sys/sysctl.h>
#include <mach/machine.h>
#else
#include <sys/sysctl.h>
#endif

CF_DECLARE(cf_cpu_type_t) cf_sys_cpu_type() {
#ifdef CF_OS_WIN
    return CF_CPU_TYPE_OTHER;
#elif defined(CF_OS_MAC)
    cf_int_t cputype;
    size_t size;
    sysctlbyname("hw.cputype", &cputype, &size, NULL, 0);
    switch(cputype) {
    case CPU_TYPE_X86: return CF_CPU_TYPE_X86;
    case CPU_TYPE_ARM: return CF_CPU_TYPE_ARM;
    case CPU_TYPE_POWERPC: return CF_CPU_TYPE_POWERPC;
    default: return CF_CPU_TYPE_OTHER;
    }
#else
    return CF_CPU_TYPE_OTHER;
#endif
}
CF_DECLARE(cf_cpu_arch_t) cf_sys_cpu_arch() {
    return 0;
}

CF_DECLARE(cf_int_t) cf_sys_cpu_core_count() {
#ifdef CF_OS_WIN
    return 0;
#else
    int count = 0;
    size_t size = sizeof(int);
    sysctlbyname("machdep.cpu.core_count", &count, &size, NULL, 0);
    return count;
#endif
}

CF_DECLARE(cf_int_t) cf_sys_cpu_thread_count() {
#ifdef CF_OS_WIN
    return 0;
#else
    int count = 0;
    size_t size = sizeof(int);
    sysctlbyname("machdep.cpu.thread_count", &count, &size, NULL, 0);
    return count;
#endif
}

CF_DECLARE(cf_int_t) cf_sys_cpu_freq_mhz() {
#ifdef CF_OS_WIN
    return 0;
#else
    cf_uint64_t freq = 0;
    size_t size = sizeof(cf_uint64_t);
    sysctlbyname("hw.cpufrequency", &freq, &size, NULL, 0);
    return (cf_int_t)(freq /1000000);
#endif
}

CF_DECLARE(cf_void_t) cf_sys_cpu_brand(cf_char_t* buf, cf_size_t size) {
#ifdef CF_OS_WIN
    return ;

#else
    if(!buf) return ;
    sysctlbyname("machdep.cpu.brand_string", buf, &size, NULL, 0);
#endif
}

CF_DECLARE(cf_int_t) cf_sys_mem_size_m() {
#ifdef CF_OS_WIN
    return 0;
#else 
    cf_uint64_t mem;
    size_t size = sizeof(cf_uint64_t);
    sysctlbyname("hw.memsize", &mem, &size, NULL, 0);
    return (cf_int_t)(mem >> 20);
#endif
}

CF_DECLARE(cf_void_t) cf_sys_hostname(cf_char_t* buf, cf_size_t size) {
#ifdef CF_OS_WIN
    return ;
#else 
    if(!buf) return;
    sysctlbyname("kern.hostname", buf, &size, NULL, 0);
#endif
}
