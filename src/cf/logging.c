#include "cf/logging.h"

cf_logger_t* cf_logger_new() {
    return CF_NULL_PTR;
}

void cf_logger_delete(cf_logger_t* self) {

}

void cf_logger_write(
    cf_logger_t*        self,
    const cf_char_t*    fn,
    cf_int_t            line,
    const cf_char_t*    func,
    cf_log_level_t      level,
    const cf_char_t*    fmt,
                        ...
) {

}

void cf_logger_set_level(cf_logger_t* self, cf_log_level_t level) {

}
