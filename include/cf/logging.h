/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    log.h
 * @brief   write log to log file
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2023-3-10
 ****************************************************/
#ifndef __CF_LOGING_H__
#define __CF_LOGING_H__

#include "cf/types.h"

#if (defined _MSC_VER) && (_MSC_VER < 1300)
/* vc 6 */
#define define CF_LOG(log, level, fmt, ...)
#else
#   define CF_LOG(log, level, fmt, ...)  cf_logger_write(log, __FILE__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)
#endif

/**
 * Log level.
 * lower levels are include in high levels.
 */
typedef enum {
    CF_LOG_LNONE    = 0,
    CF_LOG_LDEBUG   = 1,
    CF_LOG_LINFO    = 2,
    CF_LOG_LWARNING = 3,
    CF_LOG_LERROR   = 4,
    CF_LOG_LFATAL   = 5,
} cf_log_level_t;

typedef struct cf_logger cf_logger_t;
typedef struct cf_logsink cf_logger_sink_t;

CF_DECLS_BEGIN

/**
 * create a logger instance.
 *
 * logger instance should be delete by `cf_logger_delete`
 */
cf_logger_t* cf_logger_new();

/**
 * destroy a logger instance.
 */
void cf_logger_delete(cf_logger_t* self);

/**
 * write log.
 *
 * @param self  this pointer like c++
 * @param fn    filename
 * @param line  line number
 * @param func  function name
 * @param level log level
 * @param fmt   log format
 * @return void
 */
void cf_logger_write(
    cf_logger_t*        self,
    const cf_char_t*    fn,
    cf_int_t            line,
    const cf_char_t*    func,
    cf_log_level_t      level,
    const cf_char_t*    fmt,
                        ...
);

/**
 * set default logger level
 *
 * @param self  this pointer like c++
 * @param level log level
 * @return void
 */
void cf_logger_set_level(cf_logger_t* self, cf_log_level_t level);

/**
 * create a file logger sink
 *
 * @return a new logger sink
 */
cf_logger_sink_t* cf_logger_sink_new_type_file();

/**
 * create a terminal logger sink
 * 
 * @return a new logger sink
 */
cf_logger_sink_t* cf_logger_sink_new_type_term();

/**
 * destroy a logger sink
 */
void cf_logger_sink_delete(cf_logger_sink_t* self);

CF_DECLS_END

#endif /* __CF_LOGING_H__ */

