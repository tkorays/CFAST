#include <cf/log.h>
#include <cf/types.h>
#include <cf/memory.h>
#include <cf/thread.h>
#include <cf/str.h>
#include <cf/err.h>
#include <stdarg.h>
#include <stdio.h>

/*
F	 B
30      40      黑
31      41      紅
32      42      綠
33      43      黃
34      44      藍
35      45      紫紅
36      46      靛藍
37      47      白 
*/
#define CF_LOG_COMMON_FORMAT "[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\n"
#define CF_LOG_CLR_I_FORMAT "\e[32;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"
#define CF_LOG_CLR_E_FORMAT "\e[31;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"
#define CF_LOG_CLR_W_FORMAT "\e[33;1m[%s][P(%u)|T(%u)][%s][%s:%d, %s] %s\e[0m\n"

typedef struct {
    cf_int_t    level;
    cf_char_t*  name;
} cf_log_level_info_t;

static cf_char_t* _unknown_level_name = "U";
static cf_log_level_info_t _log_info[] = {
    {CF_LOG_LEVEL_DEBUG, "D"},
    {CF_LOG_LEVEL_INFO, "I"},
    {CF_LOG_LEVEL_WARNING, "W"},
    {CF_LOG_LEVEL_ERROR, "E"},
    {CF_LOG_LEVEL_FATAL, "F"}
};

static const cf_char_t* _cf_log_get_level_name(cf_log_level_t level) {
    cf_uint32_t i = 0;
    for(i = 0; i < CF_ARRAY_SIZE(_log_info); i++) {
        if(_log_info[i].level == level) return _log_info[i].name;
    }
    return _unknown_level_name;
}

static cf_void_t _general_output_func(struct cf_log_writer_s* log, 
                                const cf_char_t* fn, 
                                cf_int_t line, 
                                const cf_char_t* func,
                                cf_log_level_t level,
                                const cf_char_t* buf) {
    cf_char_t ts[32] = {0};
    time_t t = time(CF_NULL_PTR);
    cf_char_t* pt = ctime(&t);
    const cf_char_t* fmt = CF_LOG_COMMON_FORMAT;

    if(level < log->level) return ;
    

    cf_memcpy_s(ts, sizeof(ts), pt, cf_strlen(pt) - 1); /* rm \n */
    if(log->logto == CF_LOG_TO_TERMINAL && log->color) {
        switch(level) {
        case CF_LOG_LEVEL_INFO:
            fmt = CF_LOG_CLR_I_FORMAT;
            break;
        case CF_LOG_LEVEL_ERROR:
            fmt = CF_LOG_CLR_E_FORMAT;
            break;
        case CF_LOG_LEVEL_FATAL:
            fmt = CF_LOG_CLR_E_FORMAT;
            break;
        case CF_LOG_LEVEL_WARNING:
            fmt = CF_LOG_CLR_W_FORMAT;
            break;
        default:
            break;
        }
    } 
    if(log->uselock) cf_mutex_lock(&log->lock);
    fprintf((FILE*)log->fp, fmt, ts,
        cf_getpid(), cf_gettid(),
        _cf_log_get_level_name(level),
        fn, line, func,
        buf);
    if(log->logto == CF_LOG_TO_FILE) fflush(log->fp);
    if(log->uselock) cf_mutex_unlock(&log->lock);
}

CF_DECLARE(cf_errno_t) cf_log_create(cf_log_writer_t** log) {
    if(!log) return CF_EPARAM;

    *log = (cf_log_writer_t*)cf_malloc(sizeof(cf_log_writer_t));
    if(!*log) return CF_EMALLOC;

    cf_memset_s(*log, sizeof(cf_log_writer_t), 0, sizeof(cf_log_writer_t));
    (*log)->logto   = CF_LOG_TO_TERMINAL;
    (*log)->fp      = (cf_void_t*)stdout;
    (*log)->level   = CF_LOG_LEVEL_DEBUG;
    (*log)->nocache = CF_TRUE;
    (*log)->color   = CF_FALSE;
    (*log)->output  = _general_output_func;
    (*log)->start   = CF_TRUE;
    (*log)->uselock = CF_FALSE;
    (*log)->filesize= CF_LOG_FILE_DEF_SIZE;
    return CF_OK;
}
CF_DECLARE(cf_errno_t) cf_log_destroy(cf_log_writer_t* log) {
    if(!log) return CF_EPARAM;
    if(log->logto == CF_LOG_TO_FILE && log->fp) {
        fclose((FILE*)log->fp);
    } 
    if(log->uselock) cf_mutex_destroy(&log->lock);
    cf_memset_s(log, sizeof(cf_log_writer_t), 0, sizeof(cf_log_writer_t));
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_log_output(cf_log_writer_t* log, const cf_char_t* fn, cf_int_t line, 
    const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmt, ...) {
    va_list args;
    cf_char_t buf[1024] = {0};
    cf_bool_t overlap;

    if(!log || !fmt) return CF_EPARAM;
    if(!log->output) return CF_NOK;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    if(log->uselock) cf_mutex_lock(&log->lock);
    if(log->logto == CF_LOG_TO_FILE && (cf_strlen(buf) + ftell(log->fp)) > log->filesize*1000000) {
        overlap = CF_TRUE;
        fclose(log->fp);
        // 重新写
        log->fp = fopen(log->path, "w+");
    }
    log->output(log, fn, line, func, level, buf);
    if(log->uselock) cf_mutex_unlock(&log->lock);

    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_log_to_file(cf_log_writer_t* log, const cf_char_t* fn) {
    FILE* f = CF_NULL_PTR;
    if(!log || !fn) return CF_EPARAM;

    if(log->uselock) cf_mutex_lock(&log->lock);

    if(log->logto == CF_LOG_TO_FILE) {
        fclose(log->fp);
        log->fp = CF_NULL_PTR;
    }

    f = fopen(fn, "a+");
    if(!f) return CF_NOK;
    log->fp = (cf_void_t*)f;
    log->logto = CF_LOG_TO_FILE;
    cf_strcpy_s(log->path, sizeof(log->path), fn);
    
    if(log->uselock) cf_mutex_unlock(&log->lock);
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_log_to_terminal(cf_log_writer_t* log) {
    return log ? (log->fp = (cf_void_t*)stdout), CF_OK : CF_NOK;
}

CF_DECLARE(cf_errno_t) cf_log_set_level(cf_log_writer_t* log, cf_log_level_t level) {
    return log ? ((log->level = level), CF_OK) : CF_NOK;
}

CF_DECLARE(cf_errno_t) cf_log_set_nocache(cf_log_writer_t* log, cf_bool_t nocache) {
    return log ? ((log->nocache = nocache), CF_OK) : CF_NOK;
}

CF_DECLARE(cf_errno_t) cf_log_with_color(cf_log_writer_t* log, cf_bool_t color) {
    return log ? ((log->color = color), CF_OK) : CF_NOK;
}

CF_DECLARE(cf_errno_t) cf_log_with_lock(cf_log_writer_t* log, cf_bool_t lck) {
    if(!log) return CF_EPARAM;
    if(log->uselock) return CF_OK;
    (cf_void_t)cf_mutex_init(&log->lock, CF_NULL_PTR);
    log->uselock = lck;
    return CF_OK;
}

CF_DECLARE(cf_errno_t) cf_log_set_file_size(cf_log_writer_t* log, cf_uint_t size) {
    if(!log) return CF_EPARAM;
    if(log->uselock) cf_mutex_lock(&log->lock);
    log->filesize = size;
    if(log->uselock) cf_mutex_unlock(&log->lock);
    return CF_OK;
}
