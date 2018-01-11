#include <cfast/cf_log_if.h>
#include <stdio.h>

cf_log_t* glog = CF_NULL_PTR;
#define CF_LOG_DEBUG(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_DEBUG, fmtstr, ##__VA_ARGS__)
#define CF_LOG_INFO(fmtstr, ...)    CF_LOG(glog, CF_LOG_LEVEL_INFO, fmtstr, ##__VA_ARGS__)
#define CF_LOG_WARNING(fmtstr, ...) CF_LOG(glog, CF_LOG_LEVEL_WARNING, fmtstr, ##__VA_ARGS__)
#define CF_LOG_ERROR(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_ERROR, fmtstr, ##__VA_ARGS__)
#define CF_LOG_FATAL(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_FATAL, fmtstr, ##__VA_ARGS__)

int main() {
    cf_log_t* log = cf_log_create("abc.log");
    glog = log;

    //CF_LOG(log, CF_LOG_LEVEL_DEBUG, "hahha: %s, aaa", "wtf");
    //CF_LOG(log, CF_LOG_LEVEL_ERROR, "asss: %u", 909);

    CF_LOG_WARNING("warning......");
    CF_LOG_ERROR("lll: %u", 111);
    cf_log_destroy(log);
    return 0;
}