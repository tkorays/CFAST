#include "cfast/cf_log_if.h"
#include "cfast/cf_str_if.h"
#include "cfast/cf_mem_if.h"
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#if defined _WIN32 || WIN32 
    #include <process.h>
#else
    #include <unistd.h>
#endif

typedef struct cf_log_s {
    cf_char_t       filename[256];
    FILE            *fp;
    pthread_mutex_t mutex;
    cf_char_t       wbuf[1024];
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

cf_log_t*   cf_log_create(const cf_char_t* filename) {
    struct cf_log_s* log = CF_NULL_PTR;
    FILE *fp = CF_NULL_PTR;
    if(!filename || cf_strlen(filename) == 0) return CF_NULL_PTR;

    fp = fopen(filename, "a+");
    if(!fp) return CF_NULL_PTR;

    log = cf_malloc(sizeof(struct cf_log_s));
    if(!log) return CF_NULL_PTR;

    (cf_void_t)pthread_mutex_init(&log->mutex, CF_NULL_PTR);
    
    log->fp = fp;
    return (cf_log_t*)log;
}
cf_void_t   cf_log_destroy(cf_log_t* log) {
    if(!log) return ;
    (cf_void_t)pthread_mutex_destroy(&log->mutex);
    fclose(log->fp);
    cf_free(log);
}
cf_void_t   cf_log_set() {

}
cf_void_t   cf_log_put(cf_log_t* log, const cf_char_t* filename, cf_int_t line, const cf_char_t* func, cf_log_level_t level, const cf_char_t* fmtstr, ...) {
    va_list args;
    cf_int_t n = 0;
    time_t t = time(CF_NULL_PTR);
    cf_char_t* pt = ctime(&t);
    cf_char_t ts[32] = {0};
    if(!log || !filename || !line || !fmtstr || !pt) return ;
    cf_memcpy_s(ts, sizeof(ts), pt, cf_strlen(pt) - 2);
    pthread_mutex_lock(&log->mutex);
    n = sprintf(log->wbuf, "[%s][P(%u)|T(%u)][%s][%s:%d, %s]", ts, (cf_uint_t)getpid(), (cf_uint_t)pthread_self(), _cf_log_get_level_name(level), filename, line, func);
    va_start(args, fmtstr); 
    n += vsprintf(log->wbuf + n, fmtstr, args);
    log->wbuf[n] = '\n';
    log->wbuf[n+1] = '\0';

    fwrite(log->wbuf, cf_strlen(log->wbuf), 1, log->fp);
    pthread_mutex_unlock(&log->mutex);

    va_end(args); 
}
