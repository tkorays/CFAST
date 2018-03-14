/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    log.h
 * @brief   CFAST的日志模块对外接口
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 *********************************************/
#ifndef __CF_LOG_H__
#define __CF_LOG_H__

#include <cf/types.h>
#include <stdio.h>

CF_DECLS_BEGIN

#define CF_LOG_STDOUT stdout
#define CF_LOG_STDERR stderr

/**
 * Log level.
 */
typedef enum {
    CF_LOG_LEVEL_DEBUG      = 1,
    CF_LOG_LEVEL_INFO       = 2,
    CF_LOG_LEVEL_WARNING    = 3,
    CF_LOG_LEVEL_ERROR      = 4,
    CF_LOG_LEVEL_FATAL      = 5,
    CF_LOG_LEVEL_NONE       = 6
} cf_log_level_t;

/**
 * Log handle.
 */
typedef struct cf_log_s cf_log_t;

/**
 * Macro for logging.
 */
#define CF_LOG(log, level, fmtstr, ...)  cf_log_write(log, __FILE__, __LINE__, __FUNCTION__, level, fmtstr, ##__VA_ARGS__)
#define CF_LOG_POOL_STAT(log) cf_log_write_pool_info(log)
/* example */
/*
#define CF_LOG_DEBUG(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_DEBUG, fmtstr, __VA_ARGS__)
#define CF_LOG_INFO(fmtstr, ...)    CF_LOG(glog, CF_LOG_LEVEL_INFO, fmtstr, __VA_ARGS__)
#define CF_LOG_WARNING(fmtstr, ...) CF_LOG(glog, CF_LOG_LEVEL_WARNING, fmtstr, __VA_ARGS__)
#define CF_LOG_ERROR(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_ERROR, fmtstr, __VA_ARGS__)
#define CF_LOG_FATAL(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_FATAL, fmtstr, __VA_ARGS__)
*/

/**
 * Create a log instance.
 * @param file          File instance.
 * @return              Log handle.
 */
cf_log_t*   cf_log_create_on_file(FILE* f);

/**
 * Create a log instance.
 * @param filename      Log filename.
 * @return              Log handle.
 */
cf_log_t*   cf_log_create(const cf_char_t* filename);

/**
 * Destroy a log instance.
 * @param log           Log handle.
 */
cf_void_t   cf_log_destroy(cf_log_t* log);

/**
 * Set a log output level.
 * @param log           Log instance.
 * @param level         Control log's output level.
 */
cf_void_t   cf_log_set_level(cf_log_t* log, cf_log_level_t level);

/**
 * Set a log cache.
 * @param log           Log handle.
 * @param size          Size of cache.
 */
cf_errno_t  cf_log_set_cache(cf_log_t* log, cf_size_t size);

/**
 * Write a log to file.
 * @param log           Log handle.
 * @param filename      Trace filename.
 * @param line          Trace line.
 * @param func          Trace function.
 * @param level         Log level.
 * @param fmtstr        Format string.
 */
cf_void_t   cf_log_write(cf_log_t* log, const cf_char_t* filename, cf_int_t line, const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmtstr, ...);

/**
 * flush log right now.
 * @param log           Log instance.
 */
cf_void_t   cf_log_flush(cf_log_t* log);

/**
 * write pool info to log.
 * @param log           Log instance.
 */
cf_void_t   cf_log_write_pool_info(cf_log_t* log);

CF_DECLS_END

#endif /* __CF_LOG_H__ */
