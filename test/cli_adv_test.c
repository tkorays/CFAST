#include <cf/cli.h>
#include <cf/err.h>
#include <cf/log.h>
#include <stdio.h>

cf_log_t* glog = CF_NULL_PTR;
#define CF_LOG_DEBUG(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_DEBUG, fmtstr, ##__VA_ARGS__)
#define CF_LOG_INFO(fmtstr, ...)    CF_LOG(glog, CF_LOG_LEVEL_INFO, fmtstr, ##__VA_ARGS__)
#define CF_LOG_WARNING(fmtstr, ...) CF_LOG(glog, CF_LOG_LEVEL_WARNING, fmtstr, ##__VA_ARGS__)
#define CF_LOG_ERROR(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_ERROR, fmtstr, ##__VA_ARGS__)
#define CF_LOG_FATAL(fmtstr, ...)   CF_LOG(glog, CF_LOG_LEVEL_FATAL, fmtstr, ##__VA_ARGS__)
#define CF_LOG_MPOOL()              CF_LOG_POOL_STAT(glog)

cf_errno_t cmd_func1(cf_char_t* a, cf_void_t*b) {
    printf("cmd fuck!!!\n");
    return CF_OK;
}

int main(int argc, char* argv[]) {
    cf_cli_cfg_t cfg;
    cf_cli_t cli;
    glog = cf_log_create("abc.log");

    cf_cli_cfg_default(&cfg);
    if(CF_OK != cf_cli_init(&cli, &cfg)) CF_LOG_ERROR("fail to init cli!");

    cf_cli_register(&cli, "abc efg kkk", cmd_func1);

    cf_cli_input(&cli, argc - 1, argv + 1);

    cf_cli_uninit(&cli);
    return 0;
}