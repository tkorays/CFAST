/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    cli.h
 * @brief   Command line interface.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-12-15
 ****************************************************/
#ifndef __CF_SYSTEM_H__
#define __CF_SYSTEM_H__

#include <cf/types.h>

/**
 * @defgroup CF_SYSTEM system
 * utils for operation systems
 * @{
 */

CF_DECLS_BEGIN

typedef enum {
    CF_CPU_TYPE_X86,
    CF_CPU_TYPE_ARM,
    CF_CPU_TYPE_POWERPC,
    CF_CPU_TYPE_OTHER
} cf_cpu_type_t;

typedef enum {
    CF_CPU_ARCH_32,
    CF_CPU_ARCH_64,
    CF_CPU_ARCH_64_32
} cf_cpu_arch_t;

CF_DECLARE(cf_cpu_type_t) cf_sys_cpu_type();
CF_DECLARE(cf_cpu_arch_t) cf_sys_cpu_arch();
CF_DECLARE(cf_int_t) cf_sys_cpu_core_count();
CF_DECLARE(cf_int_t) cf_sys_cpu_thread_count();
CF_DECLARE(cf_int_t) cf_sys_cpu_freq_mhz();
CF_DECLARE(cf_void_t) cf_sys_cpu_brand(cf_char_t* buf, cf_size_t size);
CF_DECLARE(cf_int_t) cf_sys_mem_size_m();
CF_DECLARE(cf_void_t) cf_sys_hostname(cf_char_t* buf, cf_size_t size);

CF_DECLS_END

/** @} */

#endif /* __CF_SYSTEM_H__ */
