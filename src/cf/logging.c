#include "cf/logging.h"
#include "cf/memory.h"
#include "cf/list.h"
#include "cf/thread.h"
#include "cf/str.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static const cf_char_t* _g_log_level_name[] = {
    "_", "D", "I", "W", "E", "F"
};

#ifdef CF_OS_WIN
#   define CF_LOG_COMMON_FORMAT "[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\n"
#   define CF_LOG_CLR_I_FORMAT CF_LOG_COMMON_FORMAT
#   define CF_LOG_CLR_E_FORMAT CF_LOG_COMMON_FORMAT
#   define CF_LOG_CLR_W_FORMAT CF_LOG_COMMON_FORMAT
#else
#   define CF_LOG_COMMON_FORMAT "[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\n"
#   define CF_LOG_CLR_I_FORMAT "\e[32;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"
#   define CF_LOG_CLR_E_FORMAT "\e[31;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"
#   define CF_LOG_CLR_W_FORMAT "\e[33;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"
#endif

struct cf_logger {
    cf_list_t       sinks; 
    cf_log_level_t  level;
};

cf_logger_t* cf_logger_default() {
    static cf_logger_t* logger = CF_NULL_PTR;
    /* warn! reinit in multi thread */
    if (!logger) {
        logger = cf_logger_new();
    }
    return logger;
}

cf_logger_t* cf_logger_new() {
    cf_logger_t* logger = CF_NULL_PTR;

    logger = CF_TYPE_CAST(cf_logger_t*, cf_malloc(sizeof(cf_logger_t)));
    if (!logger) {
        return CF_NULL_PTR;
    }
    cf_membzero(logger, sizeof(cf_logger_t));

    if (!cf_list_init(&logger->sinks)) {
        cf_free(logger);
        return CF_NULL_PTR;
    }
    return logger;
}

void cf_logger_delete(cf_logger_t* self) {
    cf_list_iter_t it;
    cf_logger_sink_t* sink = CF_NULL_PTR;
    for (it = cf_list_iter_init(&self->sinks); it; it = cf_list_iter_next(it)) {
        sink = CF_TYPE_CAST(cf_logger_sink_t*, cf_list_iter_data(it));
        if (sink) {
            cf_logger_sink_delete(sink);
        }
    }
    cf_list_deinit(&self->sinks);
}

void cf_logger_write(
    cf_logger_t*        self,
    const cf_char_t*    filename,
    cf_int_t            line,
    const cf_char_t*    func,
    cf_log_level_t      level,
    const cf_char_t*    fmt,
                        ...
) {
    cf_list_iter_t it;
    cf_logger_sink_t* sink = CF_NULL_PTR;
    cf_char_t buf[1024] = {0};
    va_list args;
    cf_uint_t pid = cf_getpid();
    cf_uint_t tid = cf_gettid();

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    for (it = cf_list_iter_init(&self->sinks); it; it = cf_list_iter_next(it)) {
        sink = CF_TYPE_CAST(cf_logger_sink_t*, cf_list_iter_data(it));
        if (sink && sink->write && level >= self->level) {
            sink->write(sink->instance, pid, tid, filename, line, func, level, buf);
        }
    }
}

void cf_logger_set_level(cf_logger_t* self, cf_log_level_t level) {
    self->level = level;
}

cf_bool_t cf_logger_add_sink(cf_logger_t* self, cf_logger_sink_t* sink) {
    return cf_list_insert(&self->sinks, CF_LIST_POS_TAIL, sink);
}

void _log_to_file(
    void*               fp,
    cf_uint_t           pid,
    cf_uint_t           tid,
    const cf_char_t*    filename,
    cf_int_t            line,
    const cf_char_t*    func,
    cf_log_level_t      level,
    const cf_char_t*    msg
) {
    cf_char_t ts[32] = {0};
    time_t t = time(CF_NULL_PTR);
    cf_char_t* pt = ctime(&t);

    if (level < 0 || level > CF_LOG_LFATAL) return;

    cf_memcpy_s(ts, sizeof(ts), pt, cf_strlen(pt) - 1); /* rm \n */

    fprintf(
        CF_TYPE_CAST(FILE*, fp),
        CF_LOG_COMMON_FORMAT,
        ts,
        pid, 
        tid, 
        _g_log_level_name[level],
        filename, line, func, msg);
}

cf_logger_sink_t* cf_logger_sink_new_type_file(
    cf_file_t fp,
    CF_FN_LOGGER_SINK_CLEAR clear) {
    cf_logger_sink_t* sink = cf_malloc_z(sizeof(cf_logger_sink_t));
    if (!sink) {
        return CF_NULL_PTR;
    }
    sink->type      = "file";
    sink->instance  = CF_TYPE_CAST(void*, fp);
    sink->write     = _log_to_file;
    sink->clear     = clear;
    return sink;
}

void cf_logger_sink_delete(cf_logger_sink_t* self) {
    if (self->clear) {
        self->clear(self->instance);
    }
    cf_free(self);
}

