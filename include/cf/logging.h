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
#include "cf/file.h"

/**
 * @defgroup CF_LOGGING logging
 * logging utils
 * @{
 */

#if (defined _MSC_VER) && (_MSC_VER < 1300)
/* vc 6 */
#define define CF_LOG(log, level, fmt, ...)
#else
#if (defined __FILE_NAME__)
#   define CF_LOG(level, fmt, ...)  cf_logger_write(cf_logger_default(), __FILE_NAME__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)
#else
#   define CF_LOG(level, fmt, ...)  cf_logger_write(cf_logger_default(), __FILE__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)
#endif
#endif

CF_DECLS_BEGIN

/**
 * Log level.
 */
typedef enum {
    CF_LOG_LNONE    = 0, /* no log */
    CF_LOG_LDEBUG   = 1, /* debug level log */
    CF_LOG_LINFO    = 2, /* info level log */
    CF_LOG_LWARNING = 3, /* warning level log */
    CF_LOG_LERROR   = 4, /* error level log */
    CF_LOG_LFATAL   = 5, /* fatal level log */
} cf_log_level_t;


/**
 * logger instance
 * 
 * This struct represents a logger instance, which is responsible for writing logs to various sinks.
 * It contains a pointer to the default logger, as well as functions for creating and deleting logger instances,
 * writing logs, setting the default logger level, and adding logger sinks.
 */
typedef struct cf_logger cf_logger_t;

/**
 * logger sink instance
 */
typedef struct cf_logger_sink cf_logger_sink_t;

/**
 * logger's write function
 *
 * logger sink should implement this function
 */
typedef void(*CF_FN_LOGGER_SINK_WRITE)(
    void* instance,
    cf_uint_t           pid,
    cf_uint_t           tid,
    const cf_char_t*    filename,
    cf_int_t            line,
    const cf_char_t*    func,
    cf_log_level_t      level,
    const cf_char_t*    msg
);

/**
 * logger sinks's clear function for delete implementation `instance`
 */
typedef void(*CF_FN_LOGGER_SINK_CLEAR)(
    cf_logger_sink_t* instance
);

/**
 * users can implement your sinks
 */
struct cf_logger_sink {
    const cf_char_t*        type;       /** type of this logger sink */
    void*                   instance;   /** instance of logger sink implementation */
    CF_FN_LOGGER_SINK_WRITE write;      /** write log function */
    CF_FN_LOGGER_SINK_CLEAR clear;      /** user's clear function */
};


/**
 * Get the default logger.
 */
cf_logger_t* cf_logger_default();

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
 * @param self      this pointer like c++
 * @param filename  filename
 * @param line      line number
 * @param func      function name
 * @param level     log level
 * @param fmt       log format
 * @return void
 */
void cf_logger_write(
    cf_logger_t*        self,
    const cf_char_t*    filename,
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
 * add a logger sink.
 *
 * @param self  this pointer like c++
 * @param sink  logger sink, logger is responsible for releasing sinks' memory
 * @return TRUE if success
 */
cf_bool_t cf_logger_add_sink(cf_logger_t* self, cf_logger_sink_t* sink);

/**
 * create a logger sink with file handle.
 *
 * This is a simple sink without thread-safe feature,
 * users should implement your custom sinks.
 *
 * @param   fp      file handle which could be a stdout or a log file handle
 * @param   clear   clear implementation intance
 * @return  a new logger sink
 */
cf_logger_sink_t* cf_logger_sink_new_type_file(
    cf_file_t fp,
    CF_FN_LOGGER_SINK_CLEAR clear);

/**
 * destroy a logger sink
 */
void cf_logger_sink_delete(cf_logger_sink_t* self);

CF_DECLS_END

/** @} */

#endif /* __CF_LOGING_H__ */

