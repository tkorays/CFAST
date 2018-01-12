/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    cf_log_if.h
 * @brief   CFAST的日志模块对外接口
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-3
 *********************************************/
#ifndef __CF_LOG_IF_H__
#define __CF_LOG_IF_H__

#include "cf_def.h"

/**
 * Log level.
 */
typedef enum {
    CF_LOG_LEVEL_DEBUG      = 0x0001,
    CF_LOG_LEVEL_INFO       = 0x0002,
    CF_LOG_LEVEL_WARNING    = 0x0004,
    CF_LOG_LEVEL_ERROR      = 0x0008,
    CF_LOG_LEVEL_FATAL      = 0x0010
} cf_log_level_t;

/**
 * Log handle.
 */
typedef struct cf_log_s cf_log_t;

/**
 * Macro for logging.
 */
#define CF_LOG(log, level, fmtstr, ...)  cf_log_put(log, __FILE__, __LINE__, __FUNCTION__, level, fmtstr, ##__VA_ARGS__)
#define CF_LOG_POOL_STAT(log) cf_log_put_pool_info(log)
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
 * @param filename      Log filename.
 * @return              Log handle.
 */
cf_log_t*   cf_log_create(const cf_char_t* filename);

/**
 * Destroy a log instance.
 * @param log           Log handle.
 * @return              
 */
cf_void_t   cf_log_destroy(cf_log_t* log);

/**
 * Set a log output level.
 * @param log
 * @param level         Control log's output level.
 * @return
 */
cf_void_t   cf_log_set_output_level(cf_log_t* log, cf_log_level_t level);

/**
 * Set a log cache.
 * @param log           Log handle.
 * @param size          Size of cache.
 * @return
 */
cf_errno_t  cf_log_set_cache(cf_log_t* log, cf_size_t size);

/**
 * Put a log to file.
 * @param log           Log handle.
 * @param filename      Trace filename.
 * @param line          Trace line.
 * @param func          Trace function.
 * @param level         Log level.
 * @param fmtstr        Format string.
 * @return              Log handle.
 */
cf_void_t   cf_log_put(cf_log_t* log, const cf_char_t* filename, cf_int_t line, const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmtstr, ...);


cf_void_t   cf_log_flush(cf_log_t* log);

cf_void_t   cf_log_put_pool_info(cf_log_t* log);

#endif /* __CF_LOG_IF_H__ */
