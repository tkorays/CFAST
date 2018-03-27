#include <utils/log.h>

int main() {

    cf_log_writer_t* lw = CF_NULL_PTR;
    cf_log_create(&lw);
    cf_log_with_color(lw, CF_TRUE);
    cf_log_with_lock(lw, CF_TRUE);
    cf_log_set_level(lw, CF_LOG_LEVEL_ERROR);
    //cf_log_to_file(lw, "a.log");
    CF_LOG(lw, CF_LOG_LEVEL_DEBUG, "%u", 1024);
    CF_LOG(lw, CF_LOG_LEVEL_INFO, "HELLO WORLD!");
    CF_LOG(lw, CF_LOG_LEVEL_ERROR, "%u", 1024);
    CF_LOG(lw, CF_LOG_LEVEL_WARNING, "HELLO WORLD!");
    cf_log_destroy(lw);
}