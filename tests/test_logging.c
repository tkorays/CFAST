#include "cf/logging.h"
#include "cf/assert.h"

int main(int argc, char const *argv[]) {
    cf_logger_t* logger = cf_logger_default();
    cf_logger_sink_t* sink = cf_logger_sink_new_type_term();

    cf_logger_set_level(logger, CF_LOG_LINFO);
    cf_logger_add_sink(logger, sink);
    sink = CF_NULL_PTR;

    CF_LOG(CF_LOG_LINFO, "write log, a: %d, b: %s", 4566, "abcd");

    return 0;
}

