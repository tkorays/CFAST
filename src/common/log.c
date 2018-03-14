#include <cf/log.h>
#include <cf/str.h>
#include <cf/memory.h>
#include <cf/thread.h>
#include <cf/mpool.h>
#include <cf/list.h>
#include <cf/err.h>
#include <cf/mutex.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>


#define CFAST_LOG_FORMAT "[%s][P(%u)|T(%u)][%s][%s:%d, %s]"

/**
 * 单个log日志记录
 */
typedef struct cf_log_item_s {
    time_t          ts;
    cf_uint_t       pid;
    cf_uint_t       tid;
    cf_char_t*      filename;
    cf_uint_t       line;
    cf_char_t*      funcname;
    cf_log_level_t  level;
    cf_char_t*      logstr;
} cf_log_item_t;

typedef struct cf_log_s {
    cf_char_t       filename[256];
    FILE            *fp;
    cf_mutex_t      mutex;
    cf_char_t       wbuf[1024];
    cf_log_level_t  level;

    cf_mpool_t      *pool;      /* 缓存周期内使用一个内存池申请内存 */
    cf_size_t       cache_count;  /* 缓存计数 */
    cf_size_t       cache_size;
    cf_log_item_t   *cache_logs;      /* 历史日志使用链表链接 */
} cf_log_t;


typedef struct {
    cf_int_t    level;
    cf_char_t*  name;
} cf_log_level_info_t;

static cf_char_t* _unknown_level_name = "UNKNOWN";
static cf_log_level_info_t _log_info[] = {
    {CF_LOG_LEVEL_DEBUG, "DEBUG"},
    {CF_LOG_LEVEL_INFO, "INFO"},
    {CF_LOG_LEVEL_WARNING, "WARN"},
    {CF_LOG_LEVEL_ERROR, "ERROR"},
    {CF_LOG_LEVEL_FATAL, "FATAL"}
};

static const cf_char_t* _cf_log_get_level_name(cf_log_level_t level) {
    cf_uint32_t i = 0;
    for(i = 0; i < CF_ARRAY_SIZE(_log_info); i++) {
        if(_log_info[i].level == level) return _log_info[i].name;
    }
    return _unknown_level_name;
}

cf_log_t*   cf_log_create_on_file(FILE* f) {
    struct cf_log_s* log = CF_NULL_PTR;
    if(!f) return CF_NULL_PTR;

    log = cf_malloc(sizeof(struct cf_log_s));
    if(!log) return CF_NULL_PTR;

    (cf_void_t)cf_mutex_init(&log->mutex, CF_NULL_PTR);
    
    log->fp = f;
    log->level = CF_LOG_LEVEL_DEBUG;
    log->cache_size = 0;
    log->cache_logs = CF_NULL_PTR;
    log->pool = CF_NULL_PTR;
    return (cf_log_t*)log;
}


cf_log_t*   cf_log_create(const cf_char_t* filename) {
    FILE *fp = CF_NULL_PTR;
    if(!filename || cf_strlen(filename) == 0) return CF_NULL_PTR;

    fp = fopen(filename, "a+");
    if(!fp) return CF_NULL_PTR;

    return cf_log_create_on_file(fp);
}

cf_void_t cf_log_destroy(cf_log_t* log) {
    if(!log) return ;
    cf_log_flush(log);

    cf_mutex_lock(&log->mutex);
    if(log->cache_logs) cf_free(log->cache_logs);
    if(log->pool) cf_mpool_destroy(log->pool);
    cf_mutex_unlock(&log->mutex);
    (cf_void_t)cf_mutex_destroy(&log->mutex);
    if(log->fp != CF_LOG_STDOUT && log->fp != CF_LOG_STDERR) fclose(log->fp);
    cf_free(log);
}

cf_void_t cf_log_set_level(cf_log_t* log, cf_log_level_t level) {
    if(!log) return;
    cf_mutex_lock(&log->mutex);
    ((struct cf_log_s*)log)->level = level;
    cf_mutex_unlock(&log->mutex);
}

cf_errno_t cf_log_set_cache(cf_log_t* log, cf_size_t size) {
    if(!log) return CF_EPARAM;

    cf_mutex_lock(&log->mutex);

    if(!size) {
        /* 关闭前刷入日志 */
        cf_log_flush(log);

        cf_free(log->cache_logs);
        log->cache_logs = CF_NULL_PTR;
        log->cache_size = 0;
        log->cache_count = 0;

        if(log->pool) {
            cf_mpool_destroy(log->pool);
            log->pool = CF_NULL_PTR;
        }

        cf_mutex_unlock(&log->mutex);
        return CF_OK;
    }

    log->cache_logs = cf_malloc(sizeof(struct cf_log_item_s)*size);
    if(!log->cache_logs) return CF_EMALLOC;
    log->cache_size = size;
    log->cache_count = 0;

    log->pool = cf_mpool_create(1024);
    if(!log->pool) {
        cf_free(log->cache_logs);
        log->cache_logs = CF_NULL_PTR;
        log->cache_size = 0;

        cf_mutex_unlock(&log->mutex);
        return CF_OK;
    } 

    cf_mutex_unlock(&log->mutex);
    return CF_OK;
}

cf_void_t   cf_log_write(cf_log_t* log, const cf_char_t* filename, cf_int_t line, const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t n = 0;
    time_t t = time(CF_NULL_PTR);
    cf_char_t* pt = ctime(&t);
    cf_char_t ts[32] = {0};
    cf_log_item_t* item;

    if(!log || !filename || !line || !fmtstr || !pt) return ;
    /* 日志级别不够，则不记录 */
    if(((struct cf_log_s*)log)->level > level) return ;

    cf_memcpy_s(ts, sizeof(ts), pt, cf_strlen(pt) - 1);

    if(log->cache_size && log->cache_count >= log->cache_size) {
        /* 将日志立即刷到文件中 */
        cf_log_flush(log);
    }

    cf_mutex_lock(&log->mutex);
    if(log->cache_size && log->cache_count < log->cache_size) {
        /* 日志缓存 */
        item = &log->cache_logs[log->cache_count];
        /* 假设缓存池申请内存都是成功的！ */
        item->ts = t;
        item->pid = cf_getpid();
        item->tid = cf_gettid();
        item->level = level;
        item->filename = cf_mpool_alloc(log->pool, cf_strlen(filename) + 1);
        cf_strcpy_s(item->filename, cf_strlen(filename) + 1, filename);
        item->line = (cf_uint_t)line;
        item->funcname = cf_mpool_alloc(log->pool, cf_strlen(func) + 1);
        cf_strcpy_s(item->funcname, cf_strlen(func) + 1, func);

        va_start(args, fmtstr);
        vsprintf(log->wbuf, fmtstr, args);
        va_end(args);

        item->logstr = cf_mpool_alloc(log->pool, cf_strlen(log->wbuf) + 1);
        cf_strcpy_s(item->logstr, cf_strlen(log->wbuf) + 1, log->wbuf);

        log->cache_count++;
    } else {
        n = sprintf(log->wbuf, CFAST_LOG_FORMAT, ts, cf_getpid(), cf_gettid(), _cf_log_get_level_name(level), filename, line, func);

        va_start(args, fmtstr); 
        n += vsprintf(log->wbuf + n, fmtstr, args);
        va_end(args); 

        log->wbuf[n] = '\n';
        log->wbuf[n+1] = '\0';
        fwrite(log->wbuf, cf_strlen(log->wbuf), 1, log->fp);
        fflush(log->fp);
    }

    cf_mutex_unlock(&log->mutex);
}

cf_void_t _cf_log_put_item(cf_log_t* log, cf_log_item_t* item) {
    // no lock
    cf_char_t* pt;
    cf_char_t ts[32] = {0};
    if(!log || !item) return ;
    pt = ctime(&item->ts);
    cf_memcpy_s(ts, sizeof(ts), pt, cf_strlen(pt) - 1);
    sprintf(log->wbuf, CFAST_LOG_FORMAT "%s\n", ts, 
        item->pid, item->tid, _cf_log_get_level_name(item->level), 
        item->filename, item->line, item->funcname, item->logstr);
    fwrite(log->wbuf, cf_strlen(log->wbuf), 1, log->fp);
}

cf_void_t cf_log_flush(cf_log_t* log) {
    cf_size_t i;
    if(!log) return ;
    /* 释放内存，并申请新的 */
    cf_mutex_lock(&log->mutex);
    
    for(i = 0; i < log->cache_count; i++) {
        _cf_log_put_item(log, log->cache_logs + i);
    }

    cf_mpool_destroy(log->pool);
    log->pool = cf_mpool_create(1024);
    log->cache_count = 0;

    cf_mutex_unlock(&log->mutex);
}

cf_void_t cf_log_write_pool_info(cf_log_t* log) 
{
    cf_mpool_stat_t stat = { 0 };
    if(!log || !log->pool) return;
    cf_mpool_get_stat(log->pool, &stat);
    cf_log_write(log, __FILE__, __LINE__, __FUNCTION__, CF_LOG_LEVEL_INFO, "Pool Statistics: " \
    "block size(%u),"\
    "block num(%u),"\
    "large block num(%u),"\
    "used(%u),"\
    "unused(%u)",
    stat.blksize,
    stat.blknum,
    stat.lgblknum,
    stat.used,
    stat.unused );
}
