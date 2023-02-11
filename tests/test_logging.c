#include "cf/logging.h"
#include "cf/assert.h"

int main(int argc, char const *argv[]) {
    FILE* fp = fopen("test.log", "a+");
    cf_logger_t* logger = cf_logger_default();
    cf_logger_sink_t* sink = cf_logger_sink_new_type_file(stdout, CF_NULL_PTR);
    cf_logger_sink_t* sink1 = cf_logger_sink_new_type_file(fp, CF_NULL_PTR);

    cf_logger_set_level(logger, CF_LOG_LINFO);
    cf_logger_add_sink(logger, sink);
    cf_logger_add_sink(logger, sink1);
    sink = CF_NULL_PTR;
    sink1 = CF_NULL_PTR;

    CF_LOG(CF_LOG_LINFO, "write log, a: %d, b: %s", 4566, "abcd");

    cf_logger_delete(logger);

    return 0;
}

