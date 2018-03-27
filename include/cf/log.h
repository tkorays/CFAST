/****************************************************
 * Copyright (c) 2018, tkorays <tkorays@hotmail.com>
 * All rights reserved.
 * 
 * @file    log.h
 * @brief   一个实验性的日志库.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-3-23
 ****************************************************/
#ifndef __CF_UTILS_LOGGER_H__
#define __CF_UTILS_LOGGER_H__

#include <cf/types.h>
#include <cf/mutex.h>

CF_DECLS_BEGIN

#define CF_LOG_MAX_FILE_PATH 256
#define CF_LOG_MAX_BUFF_SIZE 1024
#define CF_LOG_FILE_DEF_SIZE 1 /* 1M */
#define CF_LOG_FILE_DEF_NUM  3
#if (defined _MSC_VER) && (_MSC_VER < 1300)
/* vc 6 */
#define define CF_LOG(log, level, fmt, ...)
#else
#   define CF_LOG(log, level, fmt, ...)  cf_log_output(log, __FILE__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)
#endif

/**
 * Log level
 */ 
typedef enum {
    CF_LOG_LEVEL_DEBUG      = 1,
    CF_LOG_LEVEL_INFO       = 2,
    CF_LOG_LEVEL_WARNING    = 3,
    CF_LOG_LEVEL_ERROR      = 4,
    CF_LOG_LEVEL_FATAL      = 5,
    CF_LOG_LEVEL_NONE       = 6
} cf_log_level_t;


typedef enum {
    CF_LOG_TO_FILE,
    CF_LOG_TO_TERMINAL,
    CF_LOG_TO_USERDEF
} cf_log_to_t;

/**
 * Not used!
 */
typedef struct {
    cf_uint_t   datatime;
    cf_char_t   filename[CF_LOG_MAX_FILE_PATH];
    cf_int_t    line;
    cf_uint_t   pid;
    cf_uint_t   tid;
    cf_char_t   info[CF_LOG_MAX_BUFF_SIZE];
} cf_log_item_t;

typedef struct cf_log_writer_s {
    cf_bool_t       start;
    cf_log_to_t     logto;
    cf_void_t*      fp;
    cf_char_t       path[CF_LOG_MAX_FILE_PATH];
    cf_log_level_t  level;
    cf_bool_t       nocache;
    cf_bool_t       color;
    cf_bool_t       uselock;
    cf_mutex_t      lock;
    cf_uint_t       filesize;
    /* 可以自定义显示方式 */
    cf_void_t       (*output)(struct cf_log_writer_s* log, 
                                const cf_char_t* fn, 
                                cf_int_t line, 
                                const cf_char_t* func,
                                cf_log_level_t level,
                                const cf_char_t* buf);
} cf_log_writer_t;

#define CF_LOG_FILTER_FLAG_LEVEL_LT     0x0001
#define CF_LOG_FILTER_FLAG_LEVEL_GT     0x0002
#define CF_LOG_FILTER_FLAG_LEVEL_EQ     0x0004
#define CF_LOG_FILTER_FALG_TAG_EQ       0x0008
#define CF_LOG_FILTER_FLAG_KEYWORD      0x0010
#define CF_LOG_FILTER_FLAG_FILENAME     0x0020
#define CF_LOG_FILTER_FLAG_TIMERANGE    0x0040
#define CF_LOG_FILTER_FLAG_PID          0x0080
#define CF_LOG_FILTER_FLAG_TID          0x0100 


// 默认全部输出，默认输出到console
CF_DECLARE(cf_errno_t) cf_log_create(cf_log_writer_t** log);
CF_DECLARE(cf_errno_t) cf_log_destroy(cf_log_writer_t* log);
CF_DECLARE(cf_errno_t) cf_log_output(cf_log_writer_t* log, const cf_char_t* fn, cf_int_t line, 
    const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmt, ...);

// 用户可以自定义输出方式
CF_DECLARE(cf_errno_t) cf_log_to_file(cf_log_writer_t* log, const cf_char_t* fn);
CF_DECLARE(cf_errno_t) cf_log_to_terminal(cf_log_writer_t* log);
CF_DECLARE(cf_errno_t) cf_log_set_level(cf_log_writer_t* log, cf_log_level_t level);
CF_DECLARE(cf_errno_t) cf_log_set_nocache(cf_log_writer_t* log, cf_bool_t nocache);
CF_DECLARE(cf_errno_t) cf_log_with_color(cf_log_writer_t* log, cf_bool_t color);
CF_DECLARE(cf_errno_t) cf_log_with_lock(cf_log_writer_t* log, cf_bool_t lck);
CF_DECLARE(cf_errno_t) cf_log_set_file_size(cf_log_writer_t* log, cf_uint_t size);

CF_DECLS_END

#endif /* __CF_UTILS_LOGGER_H__ */